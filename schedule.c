/* Fill in your Name and GNumber in the following two comment fields
 * Name: AJIT YADAV	
 * GNumber:G0121073
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "clock.h"
#include "structs.h"
#include "constants.h"




/* Schedule Count
 * - Return the number of Processes in the list.
 */
int schedule_count(Process *list) {
  /* Complete this Function */
	int count = 0;
Process *walker = list;
while(walker!=NULL)
	{         count++;    
	    walker = walker->next;   
	}
	return count;
}

/* Schedule Insert
 * - Insert the Process into the List with the Following Constraints
 *   list is a pointer to the head pointer for a singly linked list.
 *   It may begin as NULL
 *   1) Insert in the order of Ascending PIDs
 *   (eg. 1, 13, 14, 20, 32, 55)
 * Remember to set list to the new head if it changes!
 */
void schedule_insert(Process **list, Process *node) {
       Process *walker = *list;
       Process *prewalk = walker;
	if (*list == NULL)
  	{
      		*list = node;
 	 }
       else if (((*list)->next == NULL) || ((*list)->pid > node->pid))   // insertion at the beginning.
  	{
             if (((*list)->pid) > node->pid )
	     {
	       *list = node;
	       node = walker;
	       node ->next = walker->next;
	       (*list)-> next = node;
	     }
	     else if ((*list)->pid < node->pid)
	     {
              (*list)->next = node;
	     }
  	}
  	else 
       	{
   		while (walker != NULL)                          //insertion at the middle
    		{       
			prewalk = walker;
			walker = walker->next;
      				if ((node->pid) < (walker->pid))
           			{
             			node->next = prewalk->next;
             			prewalk->next = node;
				break;
	   			}
       				else if ((walker->next) == NULL)
       				{
					walker->next = node;
					break;
       				}
    		}
  	}
  return;
}

/* Schedule Terminate
 * - Unallocate the Memory for the given Node
 */
void schedule_terminate(Process *node) {
  /* Complete this Function */
    free(node);
    node = NULL;
       	return;
}

/* Schedule Generate
 * - Allocate the Memory and Create the Process from the given Variables
 *   Remember to initialize all values, including next!
 */
Process *schedule_generate(const char *name, int pid, int time_remaining, int time_last_run) {
  /* Complete this Function */  	
	Process *new = malloc(sizeof(Process));
	if (new == NULL)
	{
	return NULL;
	}
        else{
	strncpy(new->name,name,strlen(name)+1);
	new->pid = pid;
	new->time_remaining = time_remaining;
	new->time_last_run = time_last_run;
	new->next = NULL;
	return new;
	}
}


/* rmv_ll 
 *  *   Removes the selected node from the linked list.
 *   *   1) Is called by Schedule Select fuunction.
 *      */
void rmv_ll(Process ***list, Process *node)
    {   Process *walker = **list;
	Process *pre_walk = walker;
         while (walker!=NULL)
       {       pre_walk = walker;
	       walker = walker->next;
          	   if ((**list)->next == NULL && ((**list)->pid == node->pid))
	  		 {
	   			**list = NULL;
	   			break;
	   		 }    
	  	   else if (((**list)->pid == node->pid) && ((**list)->next != NULL) )
	  		{	  
	  			**list = (**list)->next;
				break;
	 	        }
	  	   else if (walker->pid == node->pid)
	  		{
	  			pre_walk->next = node->next;
				break;
	  		}
	}
    }

/* Starv _check
 * - Checks for the starving node in the list.
 *   1) Is called by Schedule Select fuunction.
 *   2) Returns the starving node back to schedule select function after completion.
 *   3) If no node is starving then just returns NULL.
 */
Process *starv_check(Process ***list,int currenttime)
{	int starv;
	Process *walker = **list;
        Process *node = NULL;
        while(walker!=NULL)
        {
                starv = currenttime-(walker->time_last_run);
                if (starv >= 5)                                    // starving condition
                {
                        if (node == NULL)
                        {
                        node = walker;
                        break;
                        }
                        else if ((starv == (currenttime-(node->time_last_run)))&&(walker->pid != node->pid))
                        {
                               if (walker->pid < node->pid)
                               { node = walker;
                               }
                        }
                        else if (starv > (currenttime-(node->time_last_run)))
                        {
                                node = walker;
                        }
                }
                walker = walker->next;
        }
return (node);                                                       // Returns the starving node to schedule function. 
}


/* Schedule Select
 *  * - Select the next Process to be run using the following conditions:
 *   *   1) The Process with the lowest time_remaining should run next.
 *    *   - If there are any ties, select the Process with the lowest PID
 *     *   2) If there is a Process that has not run in >= TIME_STARVATION, then
 *      *      select that one instead.
 *       *      (Use the function clock_get_time() to get the current time)
 *        *   - If there are any ties, select the Process with the lowest PID
 *         *   3) If the list is NULL, return NULL
 *          * - Remove the selected Process from the list
 *           * - Return the selected Process
 *            */

Process *schedule_select(Process **list) {
/* Complete This Function */
    int currenttime = clock_get_time(); 
    Process *walker = *list;
    Process *node = NULL;
    if (*list == NULL)
    {
    return NULL;
    }
    else
    {  		node = starv_check(&list,currenttime); // calling the starv function to check whether any node is starving or not.
       if (node == NULL)
	{
		walker = *list;
                node = walker;
                while (walker != NULL)
                {
                     if (node->time_remaining > walker->time_remaining)
                     {
                        node = walker;
                     }
                     else if ((node->time_remaining == walker->time_remaining) && (node->pid < walker->pid))
                     {
                       node = walker;
                     }
                     walker = walker->next;
                }
                rmv_ll(&list,node);    // Calls the rmv_ll function to remove the node from the linked list.
                return (node);
        }
	else
	{
	 rmv_ll(&list,node);           // calls the rmv_ll function to remove node from the linked list.
	 return(node);
	}     
}
}
