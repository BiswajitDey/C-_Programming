#include<iostream>
using namespace std;

class heap
{
	public:
			int data[20];
			int max;			
			int n;	
			void insert_heap(int x);
			int delete_heap(); 
			void display();
};

void heap::insert_heap(int x)
{
	int i;
	if(n==max)
	{
		cout<<"Heap full..!!";
		return;
	}
	n++;
	for(i=n;i>=1;)
	{
		if(i==1)
		{
			break;
		}
		if(x<=data[i/2])
		{
			break;
		}
		else
		{
			data[i]=data[i/2];
			i=i/2;			
		}
	}
	data[i]=x;
}

int heap::delete_heap()
{
	int x,k,j;
	if(n<1)
	{
		cout<<"\nHeap empty..!!";
		return 0; 		
	}
	x=data[1];
	k=data[n];
	n--;
	for(j=2;j<=n;j=j*2)
	{
		if(j<=n)
		{
			if(data[j]<data[j+1])
			{
				j++;
			}
			if(k>=data[j])
			{
				break;
			}
			data[j/2]=data[j];
		}
		
	}
	data[j/2]=k;
	return x;
}

void heap::display()
{
	int i;
	cout<<"Heap :-\n";
	for(i=1;i<=max;i++)
	{
		cout<<data[i]<<"\t";
	}
}

int main()
{
	int x,i,ch,temp;
	heap h;	
	h.n=0;
	cout<<">>Creating heap :-";
	cout<<"\n>>Enter no. of elements :";
		cin>>h.max;
	for(i=1;i<=h.max;i++)
	{
		cout<<"Enter data "<<i<<": ";
		cin>>x;		
		h.insert_heap(x);		
	}
	h.display();
	do
	{
	cout<<"\n****Menu****\n1.Insert element\n2.Delete element\n3.Display heap\n4.Exit\n\nEnter your choice :";
	cin>>ch;
	switch(ch)
	{
		case 1:				
				cout<<"Enter data : ";
				cin>>x;
				h.max++;
				h.insert_heap(x);
				h.display();
				break;
		
		case 2:
				temp=h.delete_heap();
				h.max--;
				if(temp!=0)
				{
					cout<<"Element deleted is :"<<temp;
				}
				break;
		case 3:
				h.display();
				break;
	}
	}while(ch!=4);
}
