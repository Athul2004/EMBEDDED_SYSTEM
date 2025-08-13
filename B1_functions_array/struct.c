# include <stdio.h>

struct Employee{
    char name[32];
    int id;
    float salary;
};

int main()
{   
    int i,ch;
    struct Employee e[5];
    for (int i=0;i<2;i++)
    {
    printf("Enter your name %d\n",i+1);
    scanf("%s",e[i].name);
    printf("Enter your ID %d\n",i+1);
    scanf("%d",&e[i].id);
    printf("Enter your Salary %d\n",i+1);
    scanf("%f",&e[i].salary);
    
    while(ch=getchar()!='\n'&&ch!=EOF)
    {
    
    }
    }
   
    for (int i=0;i<2;i++)
    {
        printf("Name %d= %s\nID %d:%d\nSalary %d=%.2f\n",i+1,e[i].name,i+1,e[i].id,i+1,e[i].salary);	
    }
    return 0;
}