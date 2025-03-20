#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LEN 100
#define STACK_LIMIT 10

// Voter structure
typedef struct Voter {
    char fullName[MAX_NAME_LEN];
    int voterID;
    int birthDay, birthMonth, birthYear;
    int hasVoted;
    struct Voter *next;
} Voter;

// Candidate structure
typedef struct Candidate {
    char fullName[MAX_NAME_LEN];
    int totalVotes;
    struct Candidate *next;
} Candidate;

// Queue for voter processing
typedef struct VoteQueue {
    Voter *person;
    struct VoteQueue *next;
} VoteQueue;

typedef struct {
    VoteQueue *head, *tail;
} Queue;

// Stack for undo
Voter* undoVoteStack[STACK_LIMIT];
int undoTop = -1;

// Function prototypes
Voter* registerVoters(int);
Candidate* registerCandidates(int);
int getAge(int, int, int);
Voter* findVoter(Voter*, int);
void votingPhase(Voter*, Candidate*, Queue*);
void showResults(Candidate*);
void sortResults(Candidate**);
void pushUndo(Voter*);
Voter* popUndo();
Queue* initQueue();
void addToQueue(Queue*, Voter*);
Voter* processQueue(Queue*);

int main() {
    int numVoters, numCandidates;

    printf("How many voters? ");
    scanf("%d", &numVoters);
    Voter *voterList = registerVoters(numVoters);

    printf("How many candidates? ");
    scanf("%d", &numCandidates);
    Candidate *candidateList = registerCandidates(numCandidates);

    Queue *voteQueue = initQueue();

    votingPhase(voterList, candidateList, voteQueue);
    showResults(candidateList);

    return 0;
}

// Register voters
Voter* registerVoters(int total) {
    Voter *first = NULL, *last = NULL;
    for (int i = 0; i < total; i++) {
        Voter *newVoter = (Voter*)malloc(sizeof(Voter));  // Fixed memory allocation
        printf("\nVoter %d Details:\n", i + 1);
        printf("Full Name: ");
        scanf(" %[^\n]", newVoter->fullName);
        printf("Voter ID: ");
        scanf("%d", &newVoter->voterID);
        printf("Date of Birth (DD MM YYYY): ");
        scanf("%d %d %d", &newVoter->birthDay, &newVoter->birthMonth, &newVoter->birthYear);
        newVoter->hasVoted = 0;
        newVoter->next = NULL;

        if (!first) first = newVoter;
        else last->next = newVoter;
        last = newVoter;
    }
    return first;
}

// Register candidates
Candidate* registerCandidates(int total) {
    Candidate *first = NULL, *last = NULL;
    for (int i = 0; i < total; i++) {
        Candidate *newCand = (Candidate*)malloc(sizeof(Candidate));  // Fixed memory allocation
        printf("\nCandidate %d Name: ", i + 1);
        scanf(" %[^\n]", newCand->fullName);
        newCand->totalVotes = 0;
        newCand->next = NULL;

        if (!first) first = newCand;
        else last->next = newCand;
        last = newCand;
    }
    return first;
}

// Calculate voter age
int getAge(int d, int m, int y) {
    int currentYear = 2025, currentMonth = 1, currentDay = 30;
    int age = currentYear - y;
    if (currentMonth < m || (currentMonth == m && currentDay < d)) age--;
    return age;
}

// Find voter
Voter* findVoter(Voter *list, int id) {
    while (list) {
        if (list->voterID == id) return list;
        list = list->next;
    }
    return NULL;
}

// Voting process
void votingPhase(Voter *voters, Candidate *candidates, Queue *q) {
    printf("\n--- Voting Starts ---\n");

    while (1) {
        int voterID;
        printf("\nEnter Voter ID (-1 to stop): ");
        scanf("%d", &voterID);
        if (voterID == -1) break;

        Voter *voter = findVoter(voters, voterID);
        if (!voter) {
            printf("Invalid voter ID. Try again.\n");
            continue;
        }
        if (voter->hasVoted) {
            printf("You already voted!\n");
            continue;
        }

        if (getAge(voter->birthDay, voter->birthMonth, voter->birthYear) < 18) {
            printf("Sorry, under 18 cannot vote.\n");
            continue;
        }

        addToQueue(q, voter);

        printf("\nCandidates:\n");
        Candidate *c = candidates;
        int idx = 1;
        while (c) {
            printf("%d. %s\n", idx++, c->fullName);
            c = c->next;
        }

        int choice;
        printf("Vote for candidate number: ");
        scanf("%d", &choice);

        c = candidates;
        for (int i = 1; i < choice && c; i++) c = c->next;

        if (!c) {
            printf("Invalid choice!\n");
        } else {
            c->totalVotes++;
            voter->hasVoted = 1;
            pushUndo(voter);
            printf("Vote recorded!\n");
        }
    }
}

// Sort candidates
void sortResults(Candidate **head) {
    Candidate *i, *j;
    for (i = *head; i; i = i->next) {
        for (j = i->next; j; j = j->next) {
            if (i->totalVotes < j->totalVotes) {
                int temp = i->totalVotes;
                char tempName[MAX_NAME_LEN];
                strcpy(tempName, i->fullName);

                i->totalVotes = j->totalVotes;
                strcpy(i->fullName, j->fullName);

                j->totalVotes = temp;
                strcpy(j->fullName, tempName);
            }
        }
    }
}

// Display results
void showResults(Candidate *candidates) {
    sortResults(&candidates);
    printf("\n--- Election Results ---\n");
    while (candidates) {
        printf("%s got %d votes.\n", candidates->fullName, candidates->totalVotes);
        candidates = candidates->next;
    }
}

// Stack functions
void pushUndo(Voter *voter) {
    if (undoTop < STACK_LIMIT - 1) undoVoteStack[++undoTop] = voter;
}

Voter* popUndo() {
    return (undoTop >= 0) ? undoVoteStack[undoTop--] : NULL;
}

// Queue functions
Queue* initQueue() {
    Queue *q = (Queue*)malloc(sizeof(Queue));
    q->head = q->tail = NULL;
    return q;
}

void addToQueue(Queue *q, Voter *v) {
    VoteQueue *node = (VoteQueue*)malloc(sizeof(VoteQueue));
    node->person = v;
    node->next = NULL;
    if (!q->tail) q->head = q->tail = node;
    else q->tail->next = node, q->tail = node;
}

Voter* processQueue(Queue *q) {
    if (!q->head) return NULL;
    VoteQueue *temp = q->head;
    Voter *v = temp->person;
    q->head = q->head->next;
    free(temp);
    return v;
}

