/* Рассчитать определённый интеграл какой-нибудь простой, интегрируемой аналитически на некотором промежутке функции f(x) методом Монте-Карло:
расчёт проводить параллельно в n-потоков некоторой программы А, генерируя в каждом из них N/n точек равномерно как по интервалу, так и по области значений.
Собирать и обрабатывать результаты в программе Б, читающей результаты из разделяемой памяти. [В случае использования независимых ячеек разделяемой памяти, 
сохранение результатов можно обеспечить без синхронизации, в случае разделяемой ячейки в разделяемой памяти - синхронизация необходима. Реализация способа хранения
влияет на результаты эксперимента 2 а,б*) (см. ниже).

Задачи, поставленные перед Вами как исследователем:
1) Оценить прирост производительности в связи с использованием n' потоков вместо 1, где n' - число физических ядер Вашей машины. 
Узнать число ядер-например, 'cat /cpu/procinfo'
2) Провести серию измерений, в которой, увеличивая число n' :
  а) пронаблюдать стагнацию роста производительности при увеличении числа потоков
  б*) определить, при каких n' (насколько больших) накладные расходы от использования многопоточности [и синхронизации, если она имела место]
  превосходят преимущества их использования.
*/
TRANSIMITTOR:

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<errno.h>
#include<math.h>
#include<sys/types.h>
#include<sys/shm.h>
#include<sys/ipc.h>
#include<time.h>
#include<sys/types.h>



#define N 1000000
int k = 0;

struct shmid_ds dota;

typedef struct 
{
	double x; 
	double y;

} data_t;



pthread_mutex_t mutex; 

void *thread(void *arg)
{
	int i;
	data_t data;

	for(i=0;i<N;i++)	
	{
	srand(time(NULL)*i);
	data.x =((double)rand())/(double)RAND_MAX;
	
	srand(time(NULL)*time(NULL)*i);
	data.y =((double)rand()) /(double)RAND_MAX;
	

	if(data.y<=(data.x)*(data.x))	
	{
		pthread_mutex_lock(&mutex);
		k++;
		pthread_mutex_unlock(&mutex);
	}
	}
	pthread_exit(NULL);
		
return 0;

}

int main()

{
	
	int i,m;
	double S;
	int result;
	pthread_t cpu [4];
	
	key_t key;
	char pathname[] = "./1.c";
	int shmid;
	float *array;

	for(i=0;i<4;i++)
	{
		result = pthread_create(&cpu[i], NULL, thread, NULL);
	
	
	if(result!=0)
	{
		printf("ERROR on thread %i create \n");
	}

	}
	
	for(m=0;m<4;m++)
	{
		result = pthread_join(cpu[m], NULL);

	}
	

	printf("Time: %ld\n", time_stop());

	printf("%f\n",clock);

	if((key = ftok(pathname,0))<0)
	{
		printf("Cannot generate the key\n");
		exit(-1);
	}
		
	if((shmid = shmget(key, sizeof(float), 0666 | IPC_CREAT | IPC_EXCL))<0)
	{
		if(errno!= EEXIST)
		{
			printf("Cannot create shared memory\n");
			exit(-1);
		}
		else
		{
			if((shmid = shmget(key, sizeof (float), 0666 | IPC_CREAT))<0)
			{
			
				printf("Cannot find the shared memory\n");
			
				exit(-1);

			}
		}

	}

	if((array = (float*)shmat(shmid, NULL, 0)) == (float *)(-1))
	{
		printf("Cannot attach the shared memory\n");
		exit(-1);
	}
	
	array[0]=((float)k)/(4*N);
	printf("I'm sender and I give reciver some info ...\n ");

	if(shmdt(array)<0)
	{
		printf("Cannot detach shared memory\n");
		exit(-1);
	}
	
		return 0;

RECEIVER:

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/errno.h>
#include<sys/shm.h>
#include<sys/ipc.h>

int main()
{
	float *array;
	int shmid;
	char path[] = "./1.c";
	key_t key;
	
	float S;// integral 
	if((key = ftok(path, 0))<0)
	{
		printf("Cannot generate the key\n");
		exit(-1);
	}

	if((shmid = shmget(key, sizeof(float), 0))<0)
	{
		printf("Cannot find the shared memory\n");
		exit(-1);
	}

	if((array = (float*)shmat(shmid, NULL, 0 )) == (float*)(-1))
	{
		printf("Cannot attach shared memory\n");
		exit(-1);
	}

	S = array[0];
	printf(" I'm reciver and I get S = %.3f from sender!", S);
	
	if(shmdt(array)<0)
	{
		printf("Cannot detach shared memory");
		exit(-1);
	}
	return 0;
}

