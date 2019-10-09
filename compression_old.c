/*******************************************************************************
 * 48430 Fundamentals of C Programming - Assignment 3, Huffman Compression
 * Name: Ray Robinson
 * Student ID: 12616292
 * Date of submission: 16/10/2019
 * 
 * This file forms part of the Substation System Databse (SSD) which has been 
 * designed and built by team 3. 
 * This compression program takes data in from the structure which holds all 
 * the substation telemetry information, builds a Huffman Tree which is a form
 * of a binary tree which utilises min heaps to replace the characters with
 * the most occurances with a binary code which has less bits than it would 
 * otherwise. 
 * Thie functionality can be reversed and return the data back to its original 
 * form also.
 * 
 * original file =   767,792 bits
 * compressed file = 462,828 bits
 * 
 * 39.72% reduction in size :)
*******************************************************************************/

/*******************************************************************************
*Header files which are required
*******************************************************************************/
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>
/* #include "debug.h"
#include "Substation.h"
#include "btree.h" */

/*******************************************************************************
*List of preprocessing directives
*******************************************************************************/
#define MAX_TREE_HT 100
#define MAXLEN 999999
#define ASCII_CODE 127
#define FILENAME "Substation_module_data2.csv"


int Huffman_Code[20];

/*******************************************************************************
*List of data structures*
*******************************************************************************/

/* A Huffman tree node  */
struct MinHeapNode { 

	/*  One of the input characters  */
	char data; 

	/*  Frequency of the character  */
	unsigned freq; 

	/*  Left and right child of this node  */
	struct MinHeapNode *left, *right; 
}; 

typedef struct MinHeapNode MinHeapNode_t;


/* 
A Min Heap: Collection of 
 min-heap (or Huffman tree) nodes  */

struct MinHeap { 

	/*  Current size of min heap  */
	unsigned size; 
/* 
	 capacity of min heap  */
	unsigned capacity; 

/* 	 Array of minheap node pointers  */
	MinHeapNode_t** array; 
}; 

typedef struct MinHeap MinHeap_t;


/* 
A Min Heap: Collection of 
 min-heap (or Huffman tree) nodes  */

struct Huffman { 

	/*  ASCII code  */
	int data; 

/*	 The Huffman code of character  */
	int Huffman_Code[MAX_TREE_HT]; 

	/* Size of Huffman code */
	int size;

}; 

typedef struct Huffman Huffman_t;

/*******************************************************************************
 * Function prototypes 
*******************************************************************************/
/*  A utility function allocate a new 
 min heap node with given character 
 and frequency of the character  */
MinHeapNode_t* newNode(char data, unsigned freq) ;

/*  A utility function to create 
 a min heap of given capacity  */
struct MinHeap* createMinHeap(unsigned capacity);

/*  A utility function to 
 swap two min heap nodes  */
void swapMinHeapNode(MinHeapNode_t** a, MinHeapNode_t** b) ;

/*  The standard minHeapify function.  */
void minHeapify(struct MinHeap* minHeap, int idx); 

/*  A utility function to check  if size of heap is 1 or not  */
int isSizeOne(struct MinHeap* minHeap); 

/*  A standard function to extract  minimum value node from heap  */
MinHeapNode_t* extractMin(MinHeap_t* minHeap) ;

/* A utility function to insert  a new node to Min Heap  */
void insertMinHeap(MinHeap_t* minHeap, MinHeapNode_t* MinHeapNode_t);

/*  A standard function to build min heap  */
void buildMinHeap(struct MinHeap* minHeap); 

/*  A utility function to print an array of size n  */
Huffman_t printArr(int str[], int n, int freq,char data);

/*  A utility function to store Huffman codes  */
Huffman_t write_Huff_array(int code[], int n, int ASCII, int freq);

/*Update*/
void displayflight (Huffman_t code1);

/* Utility function to check if this node is leaf  */
int isLeaf(MinHeapNode_t* root);

/*  Creates a min heap of capacity equal to size and inserts all character of 
 data[] in min heap. Initially size of min heap is equal to capacity  */
MinHeap_t* createAndBuildMinHeap(char data[], int freq[], int size) ;

/*  The main function that builds Huffman tree  */
MinHeapNode_t* buildHuffmanTree(char data[], int freq[], int size);

/*  Prints huffman codes from the root of Huffman Tree. 
 	It uses str[] to store codes  */
void printCodes(MinHeapNode_t* root, int str[], int top, int size, Huffman_t tree[], int* row);

/*  The main function that builds a Huffman Tree and print codes by
    traversing the built Huffman Tree  */
void HuffmanCodes(char data[], int count[], int size, Huffman_t tree[]);

/*******************************************************************************
 * Main
*******************************************************************************/

/*  Main function which takes input from user UPDATE */

int main() 
{ 	
	/*Maximum amount of characters which can be processed*/
	char str_raw[1000000];
	/*The number each of ASCII characters occurances in the data set used*/
	int count_raw[ASCII_CODE];
	int i;


	

	
	/*Update*/

	/*This is only required of keyboard input is used as it was writing to stdin*/
	/*fflush(stdin);*/
	/*printf("Enter a word>\n");*/
	/*fgets(str_raw,MAXLEN,stdin);*/

 	/*Update as required, once data comes from program this 
	* can go or be left for debug
	*/

	/*Open file  in read mode to read in data until EOF is reached*/ 
	FILE *textp;
	textp=fopen(FILENAME,"r");
	for (i=0;i<2000;i++)
		while (fgets(str_raw,MAXLEN,textp)!= NULL);
	fclose(textp); 	

/* 	do {
		c = fgetc(textp);

		if (feof(textp))
			break;
		printf("%s",c);
		
	}	while(1); */
/* 	printf("\n");

	fclose(textp);
	
	for (i=0;i<10;i++){
		str_raw[i]=c;
		printf("%s",str_raw);
	} */
	

	printf("Data compression is underway\n");

	/*Start processing data strings now..*/

	/* This sets the raw count of all characters in an array to 0 to begin with  */
    for(i=0;i<ASCII_CODE;i++)
    {
        count_raw[i]=0;
	}

    /*inits i to 0, for less than str length it will add 
	* a value to the ASCII index of letter found 
	*/
    for(i=0;i<strlen(str_raw);i++)
    {
        count_raw[(int)str_raw[i]] ++;
        #ifdef DEBUG
		/*prints the string which was entered and the count at that point*/
			printf("%c %d\n",str_raw[i],count_raw[(int)str_raw[i]]);
		#endif	
    }
	
	
 	#ifdef DEBUG 
	 for(i=0;i<ASCII_CODE;i++)
	{
		if (count_raw[(int)str_raw[i]]>0)
			printf("char= %c count= %d\n",str_raw[i],count_raw[(int)str_raw[i]]);
	
	} 
	
	 #endif

	
	
		
/* 	for(i=0;i<strlen(str);i++)
    {
        if(count[(int)str[i]] > 0)
        {
            #ifdef DEBUG
				printf("(loop)character: %c, Occurrences: %d\n",str[i],count[(int)str[i]]);
			#endif
            count[(int)str[i]]  = 0;
        } 
		

    }
	*/
	
	/*Loop which prints the size in bytes and ASCII code of each character */
	/*     for(i=0;i<strlen(str_raw);i++)
	 {
		printf("size = %ld char = %d\n ",sizeof str_raw[i],str_raw[i]); 

	 } 
	*/

	
	/*Loop which is used to find out how many unique characters were entered
	* by checking if the character occurs atleast once, if it doesn't it 
	* keeps searching through the array, if it does occur atleast once it
	* increments the size which determines the size of the consolidated arrays
	* used in the next part of the program.
	*/
	int size=0;
	int j=0;
	for (j=0;j<ASCII_CODE;j++){
		if (count_raw[j]>0){
		size++;
		
		#ifdef DEBUG
		printf("character :%c occured atleast once\n",str_raw[j]);
		/*Used to show the count of each character found */
		printf("number of occurances are %d\n",count_raw[(int)str_raw[j]]);
		printf("size of consoldated array is = %d\n",size);
		#endif
		

		}
		
	}

	/*The compression algorithm only needs to know which characters are present and their
	* number of occurances. Here we cut down from an array of 256 (ASCII characters avail)
	* to an array of size as determined by the characters only present.
	*/

	/*Arrays for consolidated count and characters (str) which are initialised to the size
	* as determined above.*/
	int count_consol[size];
	char str_consol[size];

	/*Loop through all 255 characters in the original array, if the count is equal to 
	* atleast 1 transfer it accross to the next available element of the consolidated
	* array for both the character and its count.
	*/
	i=0;
	for(j=0;j<ASCII_CODE;j++){
		/*printf("%d\n",(int)count_raw[j]);*/
		if ((int)count_raw[j] !=0) {
			count_consol [i]= count_raw[j];
			str_consol[i] = j;
			i++;
		}
	}

	

	/*Debugging print statement to show that the raw count and 
	* char have been transferred into the cut down colsolidated array.
	*/
	
	for(i=0;i<size;i++){
		printf("consolidated chars are %c",str_consol[i]);
		printf(" and they occur %d times\n",count_consol[i]);
		
	}	
	
	
	int count_tot=0;
	int tot_bits=0;
	for (i=0;i<size;i++){
		if (count_consol!=0)
		count_tot = count_tot+count_consol[i];

	}
			tot_bits = (count_tot*8);
		printf("Total bits = %d\n",tot_bits);
	/*#endif*/

	printf("Total chars are - %d\n",count_tot);
	printf("\n");

	/*for(i=0;i<ASCII_CODE;i++)
		if(count_raw>0)*/

 		/*printf("count %d\n",i); */

	

	/*Send off the relvant consolidated data to the Huffman functions 
	 *to build out tree and export data to the encryption step*/

	Huffman_t tree[size];
	HuffmanCodes(str_consol,count_consol,size,tree);
	/* Test print returned data */
	printf("***************************************\n******************************************");
	for(i=0; i<size; i++)
	{
		printf("Data = %c, Huffman Code = ", tree[i].data);
		for(j=0; j<tree[i].size; j++)
		{
			printf("%d", tree[i].Huffman_Code[j]);
		}
		printf("\n");
	}
	printf("***************************************\n******************************************");
	
	int k=0;
	for (i=0;i<strlen(str_raw);i++){
		printf("raw = %s\n",str_raw);}
		for (j=0;j<size;j++){
			printf("str_raw = %c & tree = %d\n",str_raw[i],tree[j].data);
			i++;;
			}
			
			if(str_raw[i] == (char)tree[j].data) {
			for(k=0;k<tree[j].size;k++)
			printf("%d",tree[j].Huffman_Code[k]);
			}

	printf("Data compression is complete.\n");
	return 0; 
} 

/*******************************************************************************
 * This function Update
 * inputs:
 * - update
 * outputs:
 * - update
*******************************************************************************/

MinHeapNode_t* newNode(char data, unsigned freq) 
{ 
		 MinHeapNode_t* temp 
		= (MinHeapNode_t*)malloc
(sizeof(MinHeapNode_t)); 

	temp->left = temp->right = NULL; 
	temp->data = data; 
	temp->freq = freq; 

	return temp; 
} 

/*******************************************************************************
 * This function Update
 * inputs:
 * - update
 * outputs:
 * - update
*******************************************************************************/

struct MinHeap* createMinHeap(unsigned capacity) 

{ 

	MinHeap_t* minHeap 
		= (MinHeap_t*)malloc(sizeof(MinHeap_t)); 

	/*  current size is 0  */
	minHeap->size = 0; 

	minHeap->capacity = capacity; 

	minHeap->array 
		= (MinHeapNode_t**)malloc(minHeap-> 
capacity * sizeof(MinHeapNode_t*)); 
	return minHeap; 
} 

/*******************************************************************************
 * This function Update
 * inputs:
 * - update
 * outputs:
 * - update
*******************************************************************************/

void swapMinHeapNode(MinHeapNode_t** a, MinHeapNode_t** b) 

{ 

	MinHeapNode_t* t = *a; 
	*a = *b; 
	*b = t; 
} 

/*******************************************************************************
 * This function Update
 * inputs:
 * - update
 * outputs:
 * - update
*******************************************************************************/

void minHeapify(struct MinHeap* minHeap, int idx) 

{ 

	int smallest = idx; 
	int left = 2 * idx + 1; 
	int right = 2 * idx + 2; 

	if (left < minHeap->size && minHeap->array[left]-> 
freq < minHeap->array[smallest]->freq) 
		smallest = left; 

	if (right < minHeap->size && minHeap->array[right]-> 
freq < minHeap->array[smallest]->freq) 
		smallest = right; 

	if (smallest != idx) { 
		swapMinHeapNode(&minHeap->array[smallest], 
						&minHeap->array[idx]); 
		minHeapify(minHeap, smallest); 
	} 
} 

/*******************************************************************************
 * This function Update
 * inputs:
 * - update
 * outputs:
 * - update
*******************************************************************************/

int isSizeOne(struct MinHeap* minHeap) 
{ 

	return (minHeap->size == 1); 
} 

/*******************************************************************************
 * This function Update
 * inputs:
 * - update
 * outputs:
 * - update
*******************************************************************************/

MinHeapNode_t* extractMin(MinHeap_t* minHeap) 

{ 

	MinHeapNode_t* temp = minHeap->array[0]; 
	minHeap->array[0] 
		= minHeap->array[minHeap->size - 1]; 

	--minHeap->size; 
	minHeapify(minHeap, 0); 

	return temp; 
} 

/*******************************************************************************
 * This function Update
 * inputs:
 * - update
 * outputs:
 * - update
*******************************************************************************/

void insertMinHeap(MinHeap_t* minHeap, MinHeapNode_t* MinHeapNode_t) 

{ 

	++minHeap->size; 
	int i = minHeap->size - 1; 

	while (i && MinHeapNode_t->freq < minHeap->array[(i - 1) / 2]->freq) { 

		minHeap->array[i] = minHeap->array[(i - 1) / 2]; 
		i = (i - 1) / 2; 
	} 

	minHeap->array[i] = MinHeapNode_t; 
} 

/*******************************************************************************
 * This function Update
 * inputs:
 * - update
 * outputs:
 * - update
*******************************************************************************/

void buildMinHeap(struct MinHeap* minHeap) 

{ 

	int n = minHeap->size - 1; 
	int i; 

	for (i = (n - 1) / 2; i >= 0; --i) 
		minHeapify(minHeap, i); 
} 

/*******************************************************************************
 * This function Update
 * inputs:
 * - update
 * outputs:
 * - update
*******************************************************************************/

Huffman_t printArr(int str[], int n, int freq,char data) 
{ 
	/*#ifdef DEBUG*/
	int i=0; 
	double code_count=0;
	int j=0;
	Huffman_t code1;


	printf("Huffman code:");
	for (i = 0; i < n; i++) {
		printf("%d", str[i]);
		code_count++;
	}

	code_count=(code_count*freq);	
	printf(" bit count = %.0f",code_count); 
	printf("\n"); 
	
	

#ifdef DEBUG
	printf("saving Huffman code:\n");
	printf("stored code=\n");
#endif
	for (i = 0; i < n; ++i)  {
		Huffman_Code[j] = str[i];
		j++;
		}
	
	printf("n = %d\n",n);
		
	
	printf("Code is = ");
	for (i=0;i<n;i++) {
	printf("%d", Huffman_Code[i]);
		
	}

	
	printf(" & data = %c\n",data);
	/*printf("string is %c\n",*Huffman_Code);*/
		
	
	
	printf("ASCII code is %d\n",data);


	 code1=write_Huff_array(Huffman_Code,n,data,freq);


	



	/*#endif*/
	return code1;
} 

/*******************************************************************************
 * This function Update
 * inputs:
 * - update
 * outputs:
 * - update
*******************************************************************************/

Huffman_t write_Huff_array(int code[], int n, int ASCII, int freq) 
{ 
	
	Huffman_t code1;
	/*int character_count=0;*/

	int i=0;
	int j=0;

	for (i = 0; i < n; i++)  {
		code1.Huffman_Code[i] = code[j];
		/*printf("%d\n",str[i]);*/
		j++;
		}
	code1.size = n;
	/*character_count++;*/
	/*printf("character_count = %d\n",character_count);*/

/* 	printf("Enter the ASCII code\n");
	scanf(" %d" ,&code1.data); */
	
	/*printf("written ASCII is %d & ",ASCII);*/
	printf("Stored code = ");
	for (i=0;i<n;i++) {
		printf("%d",code1.Huffman_Code[i]);
			
	}
	printf("\n");

	
		code1.data = ASCII;
		
		
	printf("stored ASCII is %d\n",code1.data);

	
	/*printf("written n = %d\n",n);*/

/* 	for (i=0;i<strlen(str_consol);i++)
		if (str_consol[i] = allchar.data)
			printf(" original %c becomes %d\n",str_consol[i],allchar.data); */

	/*Huffman_t all_codes;*/
	/*ReplaceCharWithCode (all_codes);*/




	return code1;

	

	
	

	
}

/*******************************************************************************
 * This function Update
 * inputs:
 * - update
 * outputs:
 * - update
*******************************************************************************/

void displayflight (Huffman_t code1)
{
    printf("%c %c %c  SYD %02d\n",
    code1.Huffman_Code[0],code1.Huffman_Code[1],code1.Huffman_Code[2],code1.data);    
}

/*******************************************************************************
 * This function Update
 * inputs:
 * - update
 * outputs:
 * - update
*******************************************************************************/

int isLeaf(MinHeapNode_t* root) 

{ 

	return !(root->left) && !(root->right); 
} 

/*******************************************************************************
 * This function Update
 * inputs:
 * - update
 * outputs:
 * - update
*******************************************************************************/

MinHeap_t* createAndBuildMinHeap(char data[], int freq[], int size) 

{ 

	MinHeap_t* minHeap = createMinHeap(size); 

    int i;
	for (i = 0; i < size; ++i) 
		minHeap->array[i] = newNode(data[i], freq[i]); 

	minHeap->size = size; 
	buildMinHeap(minHeap); 

	return minHeap; 
} 

/*******************************************************************************
 * This function Update
 * inputs:
 * - update
 * outputs:
 * - update
*******************************************************************************/

MinHeapNode_t* buildHuffmanTree(char data[], int freq[], int size)  

{ 
	MinHeapNode_t *left, *right, *top; 

/* 	 Step 1: Create a min heap of capacity equal to size. Initially, 
			 there are modes equal to size.  */
	MinHeap_t* minHeap = createAndBuildMinHeap(data, freq, size); 
	

/* 	 Iterate while size of heap doesn't become 1  */
	while (!isSizeOne(minHeap)) { 
		
/* 		 Step 2: Extract the two minimum freq items from min heap  */
		left = extractMin(minHeap);
		right = extractMin(minHeap); 
/* 
		 Step 3: Create a new internal node with frequency equal to the 
		 sum of the two nodes frequencies. 
		 Make the two extracted node as left and right children of this new 
		 node. Add this node to the min heap,
		 '$' is a special value for internal nodes, not used  */
		top = newNode('$', left->freq + right->freq); 

		top->left = left; 
		top->right = right; 

		insertMinHeap(minHeap, top); 
	} 

/* 	 Step 4: The remaining node is the root node and the tree is complete.  */
	return extractMin(minHeap); 
}

/*******************************************************************************
 * This function Update
 * inputs:
 * - update
 * outputs:
 * - update
*******************************************************************************/

/***Huffman_t printCodes(MinHeapNode_t* root, int str[], int top, int size) ***/
void printCodes(MinHeapNode_t* root, int str[], int top, int size, Huffman_t tree[], int* row) 

{ 
	Huffman_t code1;


/* 	 Assign 0 to left edge and recur  */
	if (root->left) { 

		str[top] = 0; 
		/***printCodes(root->left, str, top + 1,size); ***/
		printCodes(root->left, str, top + 1,size, tree, row); 
	} 

/* 	 Assign 1 to right edge and recur  */
	if (root->right) { 

		str[top] = 1; 
		/***printCodes(root->right, str, top + 1,size); ***/
		printCodes(root->right, str, top + 1,size, tree, row); 
	} 

/* 	 If this is a leaf node, then it contains one of the input 
	 characters, print the character and its code from str[]  */
	if (isLeaf(root)) { 

		/*Update*/
		printf("character - %c: occurences - %d ", root->data,root->freq); 
		code1=printArr(str, top,root->freq,root->data);
		printf("******************%d********************\n******************************************", *row);
		tree[*row] = code1;
		printf("******************%d********************\n******************************************", tree[*row].data);
		*row = *row + 1;
			
		
		/*write_Huff_array(str, top,size, root->data);*/ 

	/***return code1;***/
	
	}
	/***else return null; ***/


	

} 

/*******************************************************************************
 * This function Update
 * inputs:
 * - update
 * outputs:
 * - update
*******************************************************************************/

/***void HuffmanCodes(char data[], int count[], int size)***/
void HuffmanCodes(char data[], int count[], int size, Huffman_t tree[])

{ 
	/*printf("Size is = %d\n",size);*/
	#ifdef DEBUG
	int i;
	for(i=0;i<size;i++)
	/*if (count[i] !=0)*/
		printf("data = %c, element freq = %d\n",data[i], count[i]);

	printf("about to build tree\n");
	#endif
	/* 	 Construct Huffman Tree  */
	MinHeapNode_t* root = buildHuffmanTree(data, count, size); 
		/*printf("frequency is %d\ndata is: %c\n",root->freq,root->data);
		printf("freq step 1 ok\n");*/

		

	/* 	 Print Huffman codes using the Huffman tree built above  */
	int str[MAX_TREE_HT], top = 0; 

	/***printCodes(root, str, top, size); ***/
	int row = 0;
	printCodes(root, str, top, size, tree, &row);
	
} 


/*******************************************************************************
 * This function Update
 * inputs:
 * - update
 * outputs:
 * - update
*******************************************************************************/


/*******************************************************************************
 * This function Update
 * inputs:
 * - update
 * outputs:
 * - update
*******************************************************************************/


