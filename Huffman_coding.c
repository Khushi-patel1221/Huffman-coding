// including the libraries
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#define SIZE 150

// structure to store the struct node
struct node
{
    char ch;
    int freq;
    struct node *left;
    struct node *right;
};

// declaring an array of node
struct node *heap[SIZE];

// declaring and intialising heapSize as 0 to track the current size of the heap arrya
int heapSize = 0;

// function to insert heap node in the heap array which we initialized globally
void insertHeapArray(struct node *element)
{
    // increamenting the heapSize by 1 and assigning the passed node in last index
    heapSize++;
    heap[heapSize] = element;

    int current = heapSize;

    // swapping the child and parent if the freq of parent is more than the child's freq
    while (heap[current / 2]->freq > element->freq)
    {
        heap[current] = heap[current / 2];
        current /= 2;
    }
    heap[current] = element;
}

// function return minimum frequancy in the element and arrange the heap in needed manner
struct node *DeleteMin()
{
    // node to store the last and min element which will we need to swap in the end
    struct node *minElement, *lastElement;

    // int current and child are for the iteration
    int current;
    int child;

    //
    minElement = heap[1];
    lastElement = heap[heapSize--];
    for (current = 1; current * 2 <= heapSize; current = child)
    {
        child = current * 2;
        if (child != heapSize && heap[child + 1]->freq < heap[child]->freq)
        {
            child++;
        }
        if (lastElement->freq > heap[child]->freq)
        {
            heap[current] = heap[child];
        }
        else
        {
            break;
        }
    }
    heap[current] = lastElement;
    return minElement;
}

// function to print the huffman code table
void printTable(struct node *tree, char *code)
{
    // base case for this recursive function
    if (tree->left == NULL && tree->right == NULL)
    {
        printf("\t\t\t|\t%c\t|\t%s\t\t|\n", tree->ch, code);
        return;
    }

    char leftcode[10], rightcode[10];
    strcpy(leftcode, code);
    strcpy(rightcode, code);

    // as we move left, on our way to leaf node we count 0
    leftcode[strlen(code)] = '0';
    leftcode[strlen(code) + 1] = '\0';

    // as we move right, our way to leaf node we count 1
    rightcode[strlen(code)] = '1';
    rightcode[strlen(code) + 1] = '\0';

    // recursively calling the function printTable again
    printTable(tree->left, leftcode);
    printTable(tree->right, rightcode);
}

// function to decode the binary code
void decode(char *code, struct node *root)
{
    for (int i = 0; i < strlen(code);)
    {
        struct node *temp = root;

        while (temp != NULL)
        {
            char c = code[i];
            i++;
            if (c == '1')
            {
                temp = temp->right;
                if (temp->ch != '\0')
                {
                    printf("%c", temp->ch);
                    break;
                }
            }
            else
            {
                temp = temp->left;
                if (temp->ch != '\0')
                {
                    printf("%c", temp->ch);
                    break;
                }
            }
        }
    }
}

// main function
int main()
{
    printf("\n---------------------------------------------------------------------------------------------\n");
    printf("\n\t\t\t\tHuffman Encoding\n");
    printf("\n---------------------------------------------------------------------------------------------\n");
    // initialising the heap root node array and initialized with 0
    heap[0] = (struct node *)malloc(sizeof(struct node));
    heap[0]->freq = 0;

    // taking the input stream from the user
    char string[500];
    int total = 0;
    printf("\n\n\t\t\tEnter the input data stream:");
    fflush(stdin);
    // fgets(string,500,stdin);
    scanf("%[^\n]%*c", string);

    int noOfChar = 0;

    // constructing the frequency table
    int initialFreq[257] = {0};
    for (int i = 0; i < strlen(string); i++)
    {
        for (int j = 0; j < 257; j++)
        {
            if (string[i] == j)
            {
                initialFreq[j]++;
            }
        }
    }
    for (int i = 0; i < 257; i++)
    {
        if (initialFreq[i] != 0)
        {
            noOfChar++;
        }
    }

    // making an array for storing the freq an the characters present in the stirng entered
    char ch[noOfChar];
    int freq[noOfChar];
    int i = 0;
    for (int j = 0; j < 257; j++)
    {
        if (initialFreq[j] != 0)
        {
            freq[i] = initialFreq[j];
            ch[i] = j;
            i++;
        }
    }

    // printing the frequency table
    printf("\n---------------------------------------------------------------------------------------------\n");
    printf("\n\t\t\t\tThe Frequency Table\n");
    printf("\n---------------------------------------------------------------------------------------------\n\n");

    printf("\t\t\t|\tChar\t|\tFreq\t|\n");
    for (int k = 0; k < noOfChar; k++)
    {
        printf("\t\t\t|\t%c\t|\t%d\t|\n", ch[k], freq[k]);
        total += freq[k];
    }
    printf("\t\t\t\tTotal:\t\t%d\n", total);

    // creating the heap accoring to the huffman encoding rules
    for (int l = 0; l < noOfChar; l++)
    {
        struct node *temp = (struct node *)malloc(sizeof(struct node));
        temp->ch = ch[l];
        temp->freq = freq[l];
        temp->left = temp->right = NULL;
        insertHeapArray(temp);
    }

    // printing the huffman code table's title and one case
    printf("\n---------------------------------------------------------------------------------------------\n");
    printf("\n\t\t\t\tThe Huffman Code Table\n");
    printf("\n---------------------------------------------------------------------------------------------\n\n");
    printf("\t\t\t|\tChar\t|\tCode\t\t|\n");
    if (noOfChar == 1)
    {
        printf("\t\t\t|\t\t%s\t|\t0\t\t|\n", ch);
    }

    // creating the tree with the total as the sum of frequency of left and right child
    for (i = 0; i < noOfChar - 1; i++)
    {
        struct node *leftptr = DeleteMin();
        struct node *rightptr = DeleteMin();
        struct node *temp = (struct node *)malloc(sizeof(struct node));
        temp->ch = '\0';
        temp->left = leftptr;
        temp->right = rightptr;
        temp->freq = leftptr->freq + rightptr->freq;
        insertHeapArray(temp);
    }

    //  storing the whole created tree as struct node tree
    struct node *tree = DeleteMin();

    // printing the huffman code table by calling the printTable(struct node* tree, char code[]) function
    char code[10];
    code[0] = '\0';
    printTable(tree, code);
    printf("\n\n");

    printf("\n\t\t\t\tThe data stream before encoding: \n");
    for (int k = 0; k < strlen(string); k++)
    {
        long int decimalNumber, remainder, quotient;
        int binaryNumber[100], i = 1, j;

        decimalNumber = string[k];
        quotient = decimalNumber;

        while (quotient != 0)
        {
            binaryNumber[i++] = quotient % 2;
            quotient = quotient / 2;
        }

        /* ---- Add the following code ---- */
        {
            int group_size = 8;                                /* Or CHAR_BIT */
            int padding = group_size - ((i - 1) % group_size); /* i was inited with 1 */
            if (padding != group_size)
            {
                /* Add padding */
                while (padding-- != 0)
                    binaryNumber[i++] = 0;
            }
        }
        /* ------- Modification ends -------- */
        // printf("%d ", decimalNumber);

        for (j = i - 1; j > 0; j--)
            printf("%d", binaryNumber[j]);
    }

    printf("\n---------------------------------------------------------------------------------------------\n");
    printf("\n\t\t\t\tHuffman Decoding\n");
    printf("\n---------------------------------------------------------------------------------------------\n");
    printf("\n\t\t\t\tThe data stream after data compression: \n");
    // char code1[] = "100011110111101011010";
    char code1[100];
    printf("\n\t\t\t\tEnter the binary stream: ");
    scanf("%s", code1);
    printf("\n\t\t\t\tThe decoded text is: ");
    decode(code1, tree);
    printf("\n\n\t\t\t\tThank You for Using our Program...\n\t\t\t\t.\n\t\t\t\t.\n\t\t\t\t");
}