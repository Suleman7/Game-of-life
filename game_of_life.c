//Suleman Ahmed Butt
//'Game of Life' paralleized using MPI 

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<mpi.h>

// Function to print the output
void display(int row, int col, int **cell)
{

  int i,j;
  for(i=0;i<row;i++)
			{
				for(j=0;j<col;j++)
				{
				
			        printf("%d\t",cell[i][j]);
                                
				}
		printf("\n");
			}
		printf("\n");

}  
// function for random number
void rnum(int row, int col, int **cell, int r)
{
    //printf("Processor number %d is assigning values\n",r);
  int i,j;
  for(i=1;i<row+1;i++)
	{
		for(j=0;j<col;j++)
			{
                cell[i][j]=rand()%2;
                //printf("%d\t",cell[i][j]);
                                
			}
				//printf("\n");
	}
}
//function for updating with every timestep
void update(int row, int col, int **cell, int **cell1, int r){

    int i,j,sum,l,k,x,y;

    //printf("Processor number %d is updating values\n",r);

    for(i=1;i<row+1;i++)
    {
    for(j=0;j<col;j++)
    {
        sum=0;
    for(l=i-1;l<=i+1;l++)
        {
        for(k=j-1;k<=j+1;k++)
            {

             x=l, y=k;						// periodic boundary condition
                    /*if(x<0)
                    {
                     x = row-1;
                        }
                    else if(x>row-1)
                    {
                     x=0;
                    }
                    if(y<0)
                    {
                     y = col-1;
                    }
                    else if(y>col-1)
                    {
                     y=0;
                    }*/


            sum=sum+cell[x][y];
            cell1[i][j]=sum;         //creating a array and calculating sum of neigbours
    //		checkbounds(i,j,row,col);

            }
            }
        if(cell[i][j]==1)
                      {
                      sum--;
        }
        cell1[i][j]=sum;

        }
    }
for(i=1; i<row+1; i++)
    {
            for(j=0; j<col; j++)
            {
                    if(cell[i][j]==0)
                    {
                            if(cell1[i][j]==3)
                    {
                    cell[i][j]=1;
                    }
                    }
                    if(cell[i][j]==1)
                    {
                            if((cell1[i][j]<2)||(cell1[i][j]>3))
                    {
                    cell[i][j]=0;
                    }
                    }
            }
    }
}

int main(int argc, char *argv[])
{
    int numprocess, rank;

    MPI_Status status;
    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&numprocess);    //The communicator is MPI_COMM_WORLD
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);        //Rank of a particular process

  int i,j,k,total_row=1000,col=1000,m,time=5,sum,id1, id2, row;
  row = total_row/numprocess;
  
  int **cell,**cell1;// **final;

  cell = (int **) malloc(sizeof(int *) * (row+2));
  cell1 = (int **) malloc(sizeof(int *) * (row+2));

  for(i=0; i<row+2; i++)
  {
  cell[i] = (int *)malloc(sizeof(int) * col);
  cell1[i] = (int *)malloc(sizeof(int) * col);
  }

  rnum(row,col,cell, rank);
  //MPI_Barrier(MPI_COMM_WORLD);
  int **rbuf=NULL;

  if (rank==0){
      rbuf= (int **)malloc((total_row*col)*sizeof(int *));
  }
  MPI_Gather(&(cell[1][0]), row*col, MPI_INT, rbuf, row*col, MPI_INT, 0, MPI_COMM_WORLD);
  if (rank==0){
  //display(total_row,col,rbuf);

  }

  for(m=0;m<time;m++)
    {	if (rank == numprocess-1){
            id1=0;
        }
        else id1=rank+1;

        if (rank == 0){
            id2 = numprocess-1;
        }
        else id2 = rank-1;
        MPI_Send(&(cell[row-1][0]),col,MPI_INT,id1,201,MPI_COMM_WORLD);
        MPI_Send(&(cell[1][0]),col,MPI_INT,id2,401,MPI_COMM_WORLD);

        MPI_Recv(&(cell[0][0]),col,MPI_INT,id2,201,MPI_COMM_WORLD,&status);
        MPI_Recv(&(cell[row][0]),col,MPI_INT,id1,401,MPI_COMM_WORLD,&status);
        update(row,col,cell,cell1, rank);
        //MPI_Barrier(MPI_COMM_WORLD);

        MPI_Gather(&(cell[1][0]), row*col, MPI_INT, rbuf, row*col, MPI_INT, 0, MPI_COMM_WORLD);
        if (rank==0){

        //display(row,col,rbuf);
        }


	}

 //printf("End.\n");

 MPI_Finalize();



     for(i=0;i<row+2;i++)
	{
	 free(cell[i]);
	 free(cell1[i]);
	}
	free(cell);
    free(cell1);

}



