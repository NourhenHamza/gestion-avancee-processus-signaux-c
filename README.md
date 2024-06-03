# Gestion Avancée des Processus et des Signaux en C sous Linux

## Description

Ce projet illustre la gestion avancée des processus et des signaux en C sous Linux. Le programme crée plusieurs processus fils à partir d'un processus père, gère la communication entre ces processus à l'aide de signaux, et utilise des mécanismes de synchronisation pour coordonner les tâches des processus fils.

## Fonctionnalités

1. **Création de Processus** : Le processus père crée quatre processus fils.
2. **Communication par Signaux** : Le processus père envoie des signaux SIGUSR1 aux processus fils pour leur demander d'exécuter des tâches spécifiques.
3. **Tâches des Fils** : Les processus fils traitent les signaux reçus, effectuent des tâches complexes simulées par `sleep` et des boucles de calcul, puis envoient des signaux de confirmation SIGUSR2 au processus père.
4. **Confirmation au Père** : Le processus père affiche un message à chaque fois qu'il reçoit une confirmation d'un processus fils. Une fois toutes les confirmations reçues, il termine les processus fils.
5. **Synchronisation** : Les processus fils utilisent un sémaphore POSIX pour s'assurer qu'ils ne commencent leurs tâches qu'une fois qu'ils ont tous reçu un signal de départ.

## Utilisation

### Prérequis

- Un compilateur C (comme `gcc`)
- Permissions nécessaires pour créer des sémaphores sur votre système Linux

### Compilation

Pour compiler le programme, exécutez la commande suivante dans votre terminal :

```bash
gcc -o gestion_processus gestion_processus.c -lpthread
