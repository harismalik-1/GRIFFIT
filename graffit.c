/**
 * CSC A48 - Intro to Computer Science II, Summer 2021
 * 
 * Assignment 3 - Graffit
 * 
 * Graphs &
 * Recursion
 * Assignment
 * For
 * Freshmen
 * In
 * Toronto
 *
 * (I am so proud of that initialism.)
 * 
 * This is the program file where you will implement your solution for
 * Assignment 3. Please make sure you read through this file carefully
 * and that you understand what is provided and what you need to complete.
 * 
 * You will also need to have read the handout carefully. 
 * 
 * Parts where you have to implement functionality are clearly labeled TODO.
 * 
 * Be sure to test your work thoroughly, our testing will be extensive
 * and will check that your solution is *correct*, not only that it
 * provides functionality.
 * 
 * (c) 2020 William Song, Mustafa Quraish
 * (c) 2021 Charles Xu
 **/

#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define MAX_STR_LEN 1024

#ifndef MAT_SIZE
#define MAT_SIZE 3 // A small graph
#endif

typedef struct user_struct
{
  char name[MAX_STR_LEN];
  struct friend_node_struct *friends;
  struct brand_node_struct *brands;
  bool visited;
} User;

typedef struct friend_node_struct
{
  User *user;
  struct friend_node_struct *next;
} FriendNode;

typedef struct brand_node_struct
{
  char brand_name[MAX_STR_LEN];
  struct brand_node_struct *next;
} BrandNode;

FriendNode *allUsers = NULL;

int brand_adjacency_matrix[MAT_SIZE][MAT_SIZE];
char brand_names[MAT_SIZE][MAX_STR_LEN];

/**
 * Given the head to a FriendNode linked list, returns true if a
 * given user's name exists in the list. Returns false otherwise.
 */
bool in_friend_list(FriendNode *head, User *node)
{
  for (FriendNode *cur = head; cur != NULL; cur = cur->next)
  {
    if (strcmp(cur->user->name, node->name) == 0)
    {
      return true;
    }
  }
  return false;
}

/**
 * Given the head to a BrandNode linked list, returns true if a
 * given brand's name exists in the list. Returns false otherwise.
 */
bool in_brand_list(BrandNode *head, char *name)
{
  for (BrandNode *cur = head; cur != NULL; cur = cur->next)
  {
    if (strcmp(cur->brand_name, name) == 0)
    {
      return true;
    }
  }
  return false;
}

/**
 * Given the head to a FriendNode linked list, inserts a given user
 * into the linked list and returns the head of the new linked list.
 * The insertion is done in alphabetical order. If the user already
 * exists, no modifications are made and the list is returned as is.
 */
FriendNode *insert_into_friend_list(FriendNode *head, User *node)
{
  if (node == NULL)
    return head;

  if (in_friend_list(head, node))
  {
    printf("User already in list\n");
    return head;
  }

  FriendNode *fn = calloc(1, sizeof(FriendNode));
  fn->user = node;
  fn->next = NULL;

  if (head == NULL)
    return fn;

  if (strcmp(head->user->name, node->name) > 0)
  {
    fn->next = head;
    return fn;
  }

  FriendNode *cur;
  for (cur = head; cur->next != NULL && strcmp(cur->next->user->name, node->name) < 0;
       cur = cur->next)
    ;

  fn->next = cur->next;
  cur->next = fn;

  return head;
}

/**
 * Given the head to a BrandNode linked list, inserts a given brand into
 * the linked list and returns the head of the new linked list. The
 * insertion is done in alphabetical order. If the brand already exists,
 * no modifications are made and the list is returned as is.
 */
BrandNode *insert_into_brand_list(BrandNode *head, char *node)
{
  if (node == NULL)
    return head;

  if (in_brand_list(head, node))
  {
    printf("Brand already in list\n");
    return head;
  }

  BrandNode *fn = calloc(1, sizeof(BrandNode));
  strcpy(fn->brand_name, node);
  fn->next = NULL;

  if (head == NULL)
    return fn;

  if (strcmp(head->brand_name, node) > 0)
  {
    fn->next = head;
    return fn;
  }

  BrandNode *cur;
  for (cur = head; cur->next != NULL && strcmp(cur->next->brand_name, node) < 0;
       cur = cur->next)
    ;

  fn->next = cur->next;
  cur->next = fn;

  return head;
}

/**
 * Given the head to a FriendNode linked list, removes a given user from
 * the linked list and returns the head of the new linked list. If the user
 * does not exist, no modifications are made and the list is returned as is.
 */
FriendNode *delete_from_friend_list(FriendNode *head, User *node)
{
  if (node == NULL)
    return head;

  if (!in_friend_list(head, node))
  {
    printf("User not in list\n");
    return head;
  }

  if (strcmp(head->user->name, node->name) == 0)
  {
    FriendNode *temp = head->next;
    free(head);
    return temp;
  }

  FriendNode *cur;
  for (cur = head; cur->next->user != node; cur = cur->next)
    ;

  FriendNode *temp = cur->next;
  cur->next = temp->next;
  free(temp);

  return head;
}

/**
 * Given the head to a BrandNode linked list, removes a given brand from
 * the linked list and returns the head of the new linked list. If the brand
 * does not exist, no modifications are made and the list is returned as is.
 */
BrandNode *delete_from_brand_list(BrandNode *head, char *node)
{
  if (node == NULL)
    return head;

  if (!in_brand_list(head, node))
  {
    printf("Brand not in list\n");
    return head;
  }

  if (strcmp(head->brand_name, node) == 0)
  {
    BrandNode *temp = head->next;
    free(head);
    return temp;
  }

  BrandNode *cur;
  for (cur = head; strcmp(cur->next->brand_name, node) != 0; cur = cur->next)
    ;

  BrandNode *temp = cur->next;
  cur->next = temp->next;
  free(temp);

  return head;
}

/**
 * Given a user, prints their name, friends, and liked brands.
 */
void print_user_data(User *user)
{
  printf("User name: %s\n", user->name);

  printf("Friends:\n");
  for (FriendNode *f = user->friends; f != NULL; f = f->next)
  {
    printf("   %s\n", f->user->name);
  }

  printf("Brands:\n");
  for (BrandNode *b = user->brands; b != NULL; b = b->next)
  {
    printf("   %s\n", b->brand_name);
  }
}

/**
 * Given a brand, returns the index of the brand inside the brand_names array.
 * If it doesn't exist in the array, return -1
 */
int get_brand_index(char *name)
{
  for (int i = 0; i < MAT_SIZE; i++)
  {
    if (strcmp(brand_names[i], name) == 0)
    {
      return i;
    }
  }

  printf("Brand '%s' not found\n", name);
  return -1; // Not found
}

/**
 * Given a brand, prints their name, index (inside the brand_names
 * array), and the names of other similar brands.
 */
void print_brand_data(char *brand_name)
{
  int idx = get_brand_index(brand_name);
  if (idx < 0)
  {
    printf("Brand '%s' not in the list.\n", brand_name);
    return;
  }

  printf("Brand name: %s\n", brand_name);
  printf("Brand idx: %d\n", idx);

  printf("Similar brands:\n");
  for (int i = 0; i < MAT_SIZE; i++)
  {
    if (brand_adjacency_matrix[idx][i] == 1 && strcmp(brand_names[i], "") != 0)
    {
      printf("   %s\n", brand_names[i]);
    }
  }
}

/**
 * Read from a given file and populate a the brand list and brand matrix.
 **/
void populate_brand_matrix(char *file_name)
{
  // Read the file
  char buff[MAX_STR_LEN];
  FILE *f = fopen(file_name, "r");
  fscanf(f, "%s", buff);
  char *line = buff;
  // Load up the brand_names matrix
  for (int i = 0; i < MAT_SIZE; i++)
  {
    if (i == MAT_SIZE - 1)
    {
      strcpy(brand_names[i], line);
      break;
    }
    int index = strchr(line, ',') - line;
    strncpy(brand_names[i], line, index);
    line = strchr(line, ',') + sizeof(char);
  }
  // Load up the brand_adjacency_matrix
  for (int x = 0; x < MAT_SIZE; x++)
  {
    fscanf(f, "%s", buff);
    for (int y = 0; y < MAT_SIZE; y++)
    {
      int value = (int)buff[y * 2];
      if (value == 48)
      {
        value = 0;
      }
      else
      {
        value = 1;
      }
      brand_adjacency_matrix[x][y] = value;
    }
  }
}

/**
 * TODO: Complete this function
 * Creates and returns a user.
 */
User *create_user(char *name)
{
	User *new = NULL;
	new = (User*)calloc(1, sizeof(User));
	strcpy(new->name,name);
	new->friends = NULL;
	new->brands = NULL;
	new->visited = true;
	//printf("name = %s\n",new->name);

	allUsers = insert_into_friend_list(allUsers, new);

  	return new;
}

/**
 * TODO: Complete this function
 * Deletes a given user.
 */
int delete_user(User *user)
{
	if (in_friend_list(allUsers, user) == true)
	{
		allUsers = delete_from_friend_list(allUsers, user);

		FriendNode *p = NULL;
		p = allUsers;
		while(p != NULL)
		{
			if (in_friend_list(p->user->friends, user) == true)
			{
				p->user->friends = delete_from_friend_list(p->user->friends, user);
			}
			p = p -> next;
		}
		return 0;
	}
	else return -1;
}

/**
 * TODO: Complete this function
 * Create a friendship between user and friend.
 */
int add_friend(User *user, User *friend)
{
	if (in_friend_list(user->friends, friend) == true) return -1;

	else
	{
		if(strcmp(user->name, friend->name) != 0)
		{
			user->friends = insert_into_friend_list(user->friends, friend);
			friend -> friends = insert_into_friend_list(friend->friends, user);
		}
	}
	return 0;
}

/**
 * TODO: Complete this function
 * Removes a friendship between user and friend.
 */
int remove_friend(User *user, User *friend)
{
	
	if (in_friend_list(user->friends, friend) == false) return -1;

	else
	{
		user->friends = delete_from_friend_list(user->friends, friend);
		friend->friends = delete_from_friend_list(friend->friends, user);
	}
	return 0;
  
}

/**
 * TODO: Complete this function
 * Creates a follow relationship, the user follows the brand.
 */
int follow_brand(User *user, char *brand_name)
{
	if(in_brand_list(user->brands, brand_name) == true && get_brand_index(brand_name) == -1) return -1;
	
	else
	{
		user->brands = insert_into_brand_list(user->brands, brand_name);
	}
	return 0;
}

/**
 * TODO: Complete this function
 * Removes a follow relationship, the user unfollows the brand.
 */
int unfollow_brand(User *user, char *brand_name)
{
	if(in_brand_list(user->brands, brand_name) == false && get_brand_index(brand_name) == -1) return -1;
	
	else
	{
		user->brands = delete_from_brand_list(user->brands, brand_name);
	}
	return 0;
}

/**
 * TODO: Complete this function
 * Return the number of mutual friends between two users.
 */
int get_mutual_friends(User *a, User *b)
{
	int mutual = 0;
	FriendNode *p = NULL;

	p = a->friends;
	while(p!=NULL)
	{
		FriendNode *q = NULL;
		q = b->friends;

		while(q!=NULL)
		{
			if(strcmp(q->user->name, p->user->name)==0 && strcmp(p->user->name, a->name)!=0)
			{
				mutual++;
			}
			q = q->next;
		}
		p = p->next;
	}

	return mutual;
}

/**
 * TODO: Complete this function
 * A degree of connection is the number of steps it takes to get from
 * one user to another. Returns a non-negative integer representing
 * the degrees of connection between two users.
 */

// int helper(User *a, User *b, int degree)
// {
// 	if (strcmp(a->name, b->name)) {return degree; }

// 	FriendNode *p = NULL;
// 	p = a->friends;
// 	while(p!=NULL)
// 	{
// 		if(strcmp(p->user->name, b->name)==0 && p->user->visited == false)
// 		{
// 			p->user->visited = true;
// 			//printf("&\n");
// 			return degree; 
// 		}
// 		p = p ->next;
// 	}
// 	degree++;
// 	//printf("degree = %d\n",degree);'f'
// 	// User *q = a->friends->user;
// 	// for (;!q && q->visited == false;) 
// 	// {
// 	 	return helper(a->friends->user,b,degree);
// 	// } 

// }



int get_degrees_of_connection(User *a, User *b)
{
	// print_user_data(a);
	// int ctr = 1;
	// //printf("1234\n");
	// int degree = helper(a,b,ctr);
	// //printf("#\n");
	// //printf("d= %d\n",degree);
	//return degree;
	return 0;
}

/**
 * TODO: Complete this function
 * Marks two brands as similar.
 */
void connect_similar_brands(char *brandNameA, char *brandNameB)
{
	int index = get_brand_index(brandNameA);
	int index2 = get_brand_index(brandNameB);
	if (index == -1 || index2 == -1) return;
	else
	{
		brand_adjacency_matrix[index][index2] = 1;
		brand_adjacency_matrix[index2][index] = 1;
	}
	return;
}

/**
 * TODO: Complete this function
 * Marks two brands as not similar.
 */
void remove_similar_brands(char *brandNameA, char *brandNameB)
{
	int index = get_brand_index(brandNameA);
	int index2 = get_brand_index(brandNameB);
	if (index == -1 || index2 == -1) return;
	else
	{
		brand_adjacency_matrix[index][index2] = 0;
		brand_adjacency_matrix[index2][index] = 0;
	}
	return;

}

/**
 * TODO: Complete this function
 * Returns a suggested friend for the given user.
 */
User *get_suggested_friend(User *user)
{
	int mutual = 0;
	int ctr = -1;
	User *friend = NULL;
	FriendNode *p = NULL;
	p = allUsers;
	while(p!=NULL)
	{
		if(in_friend_list(p->user->friends, user) == false && strcmp(p->user->name, user->name)!=0)
		{
			BrandNode *b = NULL;
			b = user->brands;
			int m2 = 0;
			while(b!=NULL)
			{
				BrandNode *b2 = NULL;
				b2 = p->user->brands;
				while(b2!=NULL)
				{
					if(strcmp(b->brand_name, b2->brand_name)==0) m2++;
					b2 = b2->next;
				}
				b = b->next;
			}

			if(m2 >= mutual)
			{
				mutual = m2;
				friend = p->user;
			}
			ctr = 0;
		}

		p = p->next;
	}
	if (ctr == -1)
	{
		return NULL;
	}
	else return friend;
}

/**
 * TODO: Complete this function
 * Adds n suggested friends for the given user.
 * Returns how many friends were successfully followed.
 */
int add_suggested_friends(User *user, int n)
{
	//print_user_data(user);
	int ctr = 0;
	for (int i = 0; i < n; i++)
	{
		User *new = get_suggested_friend(user);
		//printf("Name = %s\n",new->name);
		add_friend(user, new);
		ctr++;
	}
	//print_user_data(user);
	return ctr;
}

/**
 * TODO: Complete this function
 * Follows n suggested brands for the given user.
 * Returns how many brands were successfully followed.
 */
int suggested_brand(BrandNode *user, User *x)
{
	int similar = 0;
  	int index = 0;
	for(int i = 0; i < MAT_SIZE; i++)
	{
    	int ctr = 0;
		if(in_brand_list(x->brands, brand_names[i])== false)
		{
			BrandNode *p = NULL;
			p = user;
			while(p!=NULL)
			{
				int idx = get_brand_index(p->brand_name);
				if(brand_adjacency_matrix[i][idx] == 1 && in_brand_list(user, brand_names[i]) == false)
				{
					ctr++;
				}
				p = p->next;
			}
			if (ctr>=similar && in_brand_list(x->brands, brand_names[i])== false)
			{
				if (strcmp(brand_names[index], brand_names[i]) < 0)
				{
					similar = ctr;
        			index = i;
        		}
			}
		}
	}
	return index;
}

int follow_suggested_brands(User *user, int n)
{
	//print_user_data(user);
  	BrandNode *p = user->brands;
  	char user_new[MAX_STR_LEN];
  	strcpy(user_new, "");
  	User *new_user = NULL;
  	new_user = create_user(user_new);
  	while(p!= NULL)
  	{
    	new_user->brands = insert_into_brand_list(new_user->brands, p->brand_name);
    	p = p->next;
  	}
  	free(p);
  	int i = 0;
 	while (n!=0)
 	{
		int idx = suggested_brand(new_user->brands, user);
		//printf("index = %d\n",idx);
		follow_brand(user, brand_names[idx]);
    	if(in_brand_list(user->brands, brand_names[idx])) i++;
    	n--;
    }
    //print_user_data(user);
  	delete_user(new_user);
	return i;
}



































