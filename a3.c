#include "graffit.c"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "rngs.c"
#include <math.h>
#include <time.h>

#define SIZE 10
/* 
MAT_SIZE in graffit.c should be equal to SIZE in this file

Add this line to the User struct in graffit.c:
struct friend_node_struct* userList;

Initialize user->userList = allUsers in create_user()

*/

//cd C:\Users\AndrewAucie\OneDrive\Documents\UofT\SEM 3\CSCA48\A3
int input;
void nextRandom(int value, int stream) {
    if (stream<255) {
        stream++;
        SelectStream(stream);
        return;
    }
    while (value != input) value = (long) (log(1.0 - Random()) / log(value));
    PlantSeeds(value);
    stream=0;
}

long randomize(long a, long b) {
    return (a + (long) ((b - a + 1) * Random()));
}
void print_users(FriendNode *allUsers, int n) {
    for (FriendNode *f=allUsers; f; f=f->next) {
        printf("%s | ", f->user->name);
        if (n) {
            printf("Brands: ");
            for (BrandNode *b = f->user->brands; b; b = b->next) {
                printf("%s", b->brand_name);
                if (b->next) printf(", ");
            }
            printf("\n");
        }
        else {
            for (FriendNode *p = f->user->friends; p; p = p->next) {
                printf("%s", p->user->name);
                if (p->next) printf(", ");
            }
            printf("\n");
        }
    }
    printf("\n");
}
int main()
{
    int value, stream=0;
    printf("Enter randomizer value (<0 for random): ");
    scanf("%d",&value);
    input = value;
    PlantSeeds(value);
    int brand_matrix[SIZE][SIZE];
    char brand_names[SIZE][2], brand_string[1024]="";
    for (int i=0; i<SIZE; i++) {
        brand_names[i][0] = i+65;
        brand_names[i][1] = '\0';
        strcat(brand_string, brand_names[i]);
        if (i != SIZE-1) strcat(brand_string, ",");
    }
    for (int i=0; i<SIZE; i++) {
        for (int j=0; j<SIZE; j++) {
            randomize(1,10)>4? (brand_matrix[i][j]=1):(brand_matrix[i][j]=0);
        }
    }
    for (int i=0; i<SIZE; i++) {
        brand_matrix[i][i]=0;
        for (int j=0; j<SIZE; j++) {
            brand_matrix[i][j] = brand_matrix[j][i];
        }
    }
    FILE *f = fopen("brandtest.txt","w+");
    fprintf(f, "%s\n",brand_string);
    for (int i=0; i<SIZE; i++) {
        char num_string[1024]="";
        for (int j=0; j<SIZE; j++) {
            brand_matrix[i][j]? (strcat(num_string, "1")):(strcat(num_string, "0"));
            if (j != SIZE-1) strcat(num_string, ",");
        }
        fprintf(f, "%s\n", num_string);
    }
    fclose(f);
    populate_brand_matrix("brandtest.txt");
    printf("\n-   ");
    for (int i=0; i<SIZE; i++) printf("%s ",brand_names[i]);
    printf("\n");
    for (int i=0; i<SIZE; i++) {
        printf("%s | ",brand_names[i]);
        for (int j=0; j<SIZE; j++) {
            printf("%d ",brand_matrix[i][j]);
        }
        printf("\n");
    }
    User *u0=create_user("User 0"), *u1=create_user("User 1"), *u2=create_user("User 2"), *u3=create_user("User 3");
    User *u4=create_user("User 4"), *u5=create_user("User 5"), *u6=create_user("User 6");
    User *u7=create_user("User 7"), *u8=create_user("User 8"), *u9=create_user("User 9");
    int totalUsers = 10;
    int totalSuggested = 4;
    int followed;
    FriendNode *allUsers = u1->userList;
    char tmp_brand[]="";
    tmp_brand[1]='\0';
    for (int i=0; i<SIZE; i++) {
        tmp_brand[0] = i+65;
        for (FriendNode *f=allUsers; f; f=f->next) {
            nextRandom(value, stream);
            if (randomize(1,10)>8) follow_brand(f->user, tmp_brand);
        }    
    }
    printf("\nBrand List:\n");
    print_users(allUsers, 1);
    printf("\nSUGGESTED BRANDS TEST: follow_suggested_brands(user, n)\n");
    for (FriendNode *f=allUsers; f; f=f->next) {
        nextRandom(value,stream);
        int tmp = randomize(1,10);
        if (tmp>6) {
            totalSuggested = tmp-6;
            followed = follow_suggested_brands(f->user, totalSuggested);
            printf("(%s, %d) - Suggested Brands Followed: %d\n",f->user->name, totalSuggested, followed);
        }
    }
    printf("\nUpdated Brand List:\n");
    print_users(allUsers, 1);
    for (FriendNode *u=allUsers; u; u=u->next) {
        for (FriendNode *f=allUsers; f; f=f->next) {
            nextRandom(value, stream);
            if (randomize(1,10)>8) add_friend(u->user, f->user);
        }    
    }
    printf("Friends List:\n");
    print_users(allUsers, 0);
    totalSuggested = 4;
    printf("\nSUGGESTED FRIENDS TEST: add_suggested_friends(user, n)\n");
    for (FriendNode *f=allUsers; f; f=f->next) {
        nextRandom(value, stream);
        int tmp = randomize(1,10);
        if (tmp>6) {
            totalSuggested = tmp-6;
            followed = add_suggested_friends(f->user, totalSuggested);
            printf("(%s, %d) - Suggested Friends Added: %d\n",f->user->name, totalSuggested, followed);
        }
    }
    printf("\nUpdated Friends List:\n"); 
    print_users(allUsers, 0);
    printf("\nDEGREES OF CONNECTION TEST: get_degrees_of_connection(user A, user B)\n");
    for (FriendNode *u=allUsers; u; u=u->next) {
        for (FriendNode *f=allUsers; f; f=f->next) {
            nextRandom(value, stream);
            if (randomize(1,10)==10) {
                int degree = get_degrees_of_connection(u->user, f->user);
                printf("(%s, %s): %d\n", u->user->name, f->user->name, degree);
            }
        }    
    }
}
