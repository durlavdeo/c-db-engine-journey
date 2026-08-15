#include "storage.h"

#include<stdio.h>

int write_page_to_disk(FILE *fp, Page *p, long offset)
{
    if (fseek(fp, offset, SEEK_SET) != 0)
    {
        return -1;
    }

    if (fwrite(p, sizeof(Page), 1, fp) != 1)
    {
        return -1;
    }

    return 0;
}

int append_page_to_disk(FILE *fp, Page *p)
{   
    if(fseek(fp, 0, SEEK_END) !=0)
    {
        return -1;
    }

    if (fwrite(p, sizeof(Page), 1, fp) != 1)
    {
        
        return -1;
    }

    return 0;

}

int read_page_from_disk(FILE *fp, Page *p,long offset)
{
     if (fseek(fp, offset, SEEK_SET) != 0)
    {
        return -1;
    }
    if (fread(p,sizeof(Page),1,fp) != 1)
    {
        return -1;
    }

    return 0;
}

int write_file_header(FILE *fp, FileHeader *header)
{
    if( fseek(fp, 0, SEEK_SET) !=0)
    {
        return -1;
    }
    
    if (fwrite(header, sizeof(FileHeader), 1, fp) != 1)
    {
        return -1;
    }
    
    return 0;
    
}

int read_file_header(FILE *fp, FileHeader *header)
{
     if( fseek(fp, 0, SEEK_SET) !=0)
    {
        return -1;
    }

    if (fread(header,sizeof(FileHeader),1,fp) != 1)
    {
        return -1;
    }
    
    return 0;

}

int write_page_header(FILE *fp, PageHeader *header,long offset)
{
    
    if( fseek(fp, offset, SEEK_SET) !=0)
    {
        return -1;
    }
    
    if (fwrite(header, sizeof(PageHeader), 1, fp) != 1)
    {
        return -1;
    }
    
    return 0;
}

int read_page_header(FILE *fp, PageHeader *header,long offset)
{
     if( fseek(fp, offset, SEEK_SET) !=0)
    {
        return -1;
    }

    if (fread(header,sizeof(PageHeader),1,fp) != 1)
    {
        return -1;
    }
    
    return 0;
}