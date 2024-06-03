#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>

#define NUM_CHILDREN 4

pid_t children[NUM_CHILDREN];
sem_t *sem_start;

void handle_signal(int sig) {
    pid_t pid = getpid();
    if (sig == SIGUSR1) {
        printf("Processus %d reçu le signal SIGUSR1\n", pid);
        // Simuler une tâche complexe
        sleep(2); // Simuler le travail par une pause
        for (int i = 0; i < 100000000; i++); // Boucle de calcul
        printf("Processus %d a terminé sa tâche complexe\n", pid);
        kill(getppid(), SIGUSR2); // Envoyer une confirmation au père
    }
}

void child_process(int index) {
    // Attendre que tous les processus soient prêts
    sem_wait(sem_start);
    sem_post(sem_start);

    // Configurer le gestionnaire de signal
    signal(SIGUSR1, handle_signal);

    // Attendre les signaux
    while (1) {
        pause();
    }
}

void handle_confirmation(int sig) {
    static int confirmations = 0;
    confirmations++;
    printf("Processus père a reçu une confirmation (%d/%d)\n", confirmations, NUM_CHILDREN);
    if (confirmations == NUM_CHILDREN) {
        printf("Tous les processus fils ont confirmé\n");
        for (int i = 0; i < NUM_CHILDREN; i++) {
            kill(children[i], SIGTERM); // Terminer les processus fils
        }
        exit(0);
    }
}

int main() {
    sem_start = sem_open("/sem_start", O_CREAT, 0644, 0);

    signal(SIGUSR2, handle_confirmation);

    for (int i = 0; i < NUM_CHILDREN; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            child_process(i);
            exit(0);
        } else if (pid > 0) {
            children[i] = pid;
        } else {
            perror("fork");
            exit(1);
        }
    }

    sleep(1); // Attendre que tous les processus soient prêts
    sem_post(sem_start); // Déclencher le départ simultané des processus fils

    for (int i = 0; i < NUM_CHILDREN; i++) {
        kill(children[i], SIGUSR1); // Envoyer le signal de démarrage
    }

    for (int i = 0; i < NUM_CHILDREN; i++) {
        wait(NULL); // Attendre la fin de tous les processus fils
    }

    sem_close(sem_start);
    sem_unlink("/sem_start");

    return 0;
}
