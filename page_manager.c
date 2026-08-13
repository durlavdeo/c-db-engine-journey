#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define PAGE_SIZE 256

typedef struct
{
    char data[256];
} Page;

//test struct....
//we will store 2 products per page...
//this is purely based on us to store 2 products per pages... there is no checking at all...if the total size of the products is greater than the page, it will overflow and would only store the fixed size.....
//as we advance toward more new concepts, we will learn to organize records in the pages itself....will add meta data to the page and then we will finally be able to properly delete the records in the pages as it is done in the production grade DBs...
//this page manager simply provides API to add/update page,append page and read the page from the file...
//als as you can see, we are not dealing with any data type of the data to store in the file as we did previously in the generic_file_storage....
//it is because a page is simply a fixed sized space in the file that will hold the bytes...the system doesnt need to know what is stored..it simply knows this much bytes is stored and retrieved....
//but when dealing with single record storing, we had to explicitly do the void* to allow any data type to be stored...
//but in page, we simply write tthe bytes from the memory directly to the file....the char data[256] is what is holding any data regardless of any type..it simply holds the 256bytes of data and writes to the file...
//this is waht a page is capable of and hence is important for the db...


typedef struct
{
    int productId;
    char name[80];
    float price;
    int quantity;
} Product;

int countPages(FILE *fp)
{
    fseek(fp,0,SEEK_END);
    long size = ftell(fp);
    int pageCount = size / PAGE_SIZE;
    rewind(fp);
    return pageCount;
}

int writePage(FILE *fp,int pageNum, Page *p)
{
    int pCount = countPages(fp);
    if(pageNum < 0 || pageNum > pCount-1)
    {
        printf("Invalid page number!");
        return -1;
    }

    long offset = pageNum * PAGE_SIZE;

    fseek(fp,offset,SEEK_SET);
    fwrite(p,sizeof(Page),1,fp);

    return 0;
}

int appendPage(FILE *fp,Page *srcPage)
{
    fseek(fp,0,SEEK_END);
    fwrite(srcPage,sizeof(Page),1,fp);

    return 0;

}

int readPage(FILE *fp, int pageNum, Page *dstPage)
{
    int pCount = countPages(fp);
    if(pageNum < 0 || pageNum > pCount -1)
    {
        printf("Invalid page number!");
        return -1;
    }
    long offset = pageNum * PAGE_SIZE;
    fseek(fp,offset,SEEK_SET);
    fread(dstPage,sizeof(Page),1,fp);

    return 0;
}

int main()
{
    FILE *fp = fopen("page_manager_Data.dat","w+b"); //using w+b mode to reuse the same file for the tests...
    if(fp==NULL)
    {
        printf("Error opening file!");
        exit(1);
    }
    // ================= PAGE 0 =================

    Page p0 = {0};

    Product products0[] =
    {
        {101, "Keyboard", 2500.0f, 10},
        {102, "Mouse", 1200.0f, 25}
    };

    memcpy(p0.data, products0, sizeof(products0));

    appendPage(fp, &p0);


    // ================= PAGE 1 =================

    Page p1 = {0};

    Product products1[] =
    {
        {103, "Monitor", 18500.0f, 5},
        {104, "Headphones", 4500.0f, 12}
    };

    memcpy(p1.data, products1, sizeof(products1));

    appendPage(fp, &p1);


    // ================= PAGE 2 =================

    Page p2 = {0};

    Product products2[] =
    {
        {105, "Webcam", 3500.0f, 8},
        {106, "Microphone", 7500.0f, 6}
    };

    memcpy(p2.data, products2, sizeof(products2));

    appendPage(fp, &p2);


    // ================= READ PAGE 1 =================

    Page loadedPage;

    if (readPage(fp, 1, &loadedPage) == 0)
    {
        Product loadedProducts[2];

        memcpy(
            loadedProducts,
            loadedPage.data,
            sizeof(loadedProducts)
        );

        printf("\nPAGE 1:\n");

        for (int i = 0; i < 2; i++)
        {
            printf(
                "%d | %s | %.2f | %d\n",
                loadedProducts[i].productId,
                loadedProducts[i].name,
                loadedProducts[i].price,
                loadedProducts[i].quantity
            );
        }
    }


    // ====================update PAGE 1 ===================
    Page updatedP1 = {0};

    Product updateProducts1[] =
    {
        {103, "Monitor", 20500.0f, 5},
        {104, "Headphones", 5500.0f, 12}
    };

    memcpy(updatedP1.data,updateProducts1,sizeof(updateProducts1));

    writePage(fp,1,&updatedP1);


    Page updateP1;

    if (readPage(fp, 1, &updateP1) == 0)
    {
        Product updatedProducts[2];

        memcpy(
            updatedProducts,
            updateP1.data,
            sizeof(updatedProducts)
        );

        printf("\nPAGE 1:\n");

        for (int i = 0; i < 2; i++)
        {
            printf(
                "%d | %s | %.2f | %d\n",
                updatedProducts[i].productId,
                updatedProducts[i].name,
                updatedProducts[i].price,
                updatedProducts[i].quantity
            );
        }
    }

    fclose(fp);

    return 0;
}
