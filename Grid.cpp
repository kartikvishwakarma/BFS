#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <cmath>
#include <fstream>
#include <vector>

// y-direction left--->up & right---->down
// x-direction left--->left & right---->right

//size of bucket
#define SIZE 3
using namespace std;

int min_x,min_y,max_x,max_y;


//2-D array to store points, 3 field of array,
//store data as follow:
// 0:id, 1:x-axis, 2:y-axis 
struct Bucket{
	int a[SIZE][3];
};


//Allocate memory for new bucket
struct Bucket * Create_bucket(){
	struct Bucket *bucket=new Bucket();  //(struct Bucket *)calloc(1,sizeof(struct Bucket));
	for(int i=0;i<SIZE;i++){
		bucket->a[i][0]=-1;
		bucket->a[i][1]=-1;
		bucket->a[i][2]=-1;
	}
	return bucket;
};

//Data structure for Grid directory
struct Node{
	int node_min_x;
	int node_min_y;
	int node_max_x;
	int node_max_y;
	int split_point;
	int direction;
	struct Bucket *bucket_pointer;
	struct Node *left;
	struct Node *right;
};

Node* root = new Node();

struct Node * Create_node(int x1,int y1,int x2,int y2, char var){
	struct Node *tmp=(struct Node *)calloc(1,sizeof(struct Node));
	tmp->node_min_x=x1;
	tmp->node_min_y=y1;
	tmp->node_max_x=x2;
	tmp->node_max_y=y2;
	tmp->direction=0;
	tmp->split_point=-1;
	tmp->left=NULL;
	tmp->right=NULL;
	tmp->bucket_pointer=Create_bucket();
	return tmp;
}


void Initialize(int x1,int y1,int x2,int y2, char var){
	root=(struct Node*)calloc(1,sizeof(struct Node));
	root->node_min_x=x1;
	root->node_min_y=y1;
	root->node_max_x=x2;
	root->node_max_y=y2;
	root->direction=0;
	root->split_point=-1;
	root->left=NULL;
	root->right=NULL;
	root->bucket_pointer=Create_bucket();
}



int Counter(struct Bucket *bucket, char var){
	int i=0;
	for(i=0;i<SIZE;i++)
	{
		if(bucket->a[i][0]==-1)
			break;
	}
	return i;
}


int Min_Of_X(struct Bucket *bucket, char var){
	int min=bucket->a[0][1];
	for(int i=0;i<SIZE;i++){
		if(min>=bucket->a[i][1])
			min=bucket->a[i][1];
	}
	return min;
}


int Max_Of_X(struct Bucket *bucket, char var){
	int max=bucket->a[0][1];
	for(int i=0;i<SIZE;i++){
		if(max<=bucket->a[i][1])
			max=bucket->a[i][1];
	}
	return max;
}


int Min_Of_Y(struct Bucket *bucket, char var){
	int min=bucket->a[0][2];
	for(int i=0;i<SIZE;i++){
		if(min>=bucket->a[i][2])
			min=bucket->a[i][2];
	}	
	return min;
}


int Max_Of_Y(struct Bucket *bucket, char var){
	int max=bucket->a[0][2];
	for(int i=0;i<SIZE;i++){
		if(max<=bucket->a[i][2])
			max=bucket->a[i][2];
	}
	return max;
}


void Insert_bucket(struct Bucket *bucket,int id,int x,int y, char var){
	int c=Counter(bucket,var);
	bucket->a[c][0]=id;
	bucket->a[c][1]=x;
	bucket->a[c][2]=y;
}


void Re_Order(struct Node *node,int id,int x,int y, char var){
	int d=node->direction;
	struct Bucket *bucket=node->bucket_pointer;
	if(d==1){
		int splitpoint=node->split_point;
		for(int i=0;i<SIZE;i++){
			if(bucket->a[i][1]<=splitpoint)
				Insert_bucket(node->left->bucket_pointer,bucket->a[i][0],bucket->a[i][1],bucket->a[i][2],var);
			else
				Insert_bucket(node->right->bucket_pointer,bucket->a[i][0],bucket->a[i][1],bucket->a[i][2],var);
		}
		if(x<splitpoint)
			Insert_bucket(node->left->bucket_pointer,id,x,y,var);	
		else
			Insert_bucket(node->right->bucket_pointer,id,x,y,var);
	}
	else if(d==2){
		int splitpoint=node->split_point;
		for(int i=0;i<SIZE;i++){
			if(bucket->a[i][2]>=splitpoint)
				Insert_bucket(node->left->bucket_pointer,bucket->a[i][0],bucket->a[i][1],bucket->a[i][2],var);
			else
				Insert_bucket(node->right->bucket_pointer,bucket->a[i][0],bucket->a[i][1],bucket->a[i][2],var);
		}
		if(y>=splitpoint)
			Insert_bucket(node->left->bucket_pointer,id,x,y,var);	
		else
			Insert_bucket(node->right->bucket_pointer,id,x,y,var);
	}
}


void split(struct Node *node,int id,int x,int y,int direction, char var){
	int x_minimum=Min_Of_X(node->bucket_pointer,var);
	int x_maximum=Max_Of_X(node->bucket_pointer,var);
	int y_minimum=Min_Of_Y(node->bucket_pointer,var);
	int y_maximum=Max_Of_Y(node->bucket_pointer,var);
	int x_diff=x_maximum-x_minimum;
	int y_diff=y_maximum-y_minimum;
	
	if(x_diff>=y_diff){
		node->direction=1;
		int split_x=x_minimum+floor((x_maximum-x_minimum)/2);
		node->split_point=split_x;
		node->left=Create_node(node->node_min_x,node->node_min_y,split_x,node->node_max_y,var);
		node->right=Create_node(split_x,node->node_min_y,node->node_max_x, node->node_max_y,var);
		Re_Order(node,id,x,y,var);
	}
	else if(x_diff<y_diff){
		node->direction=2;
		int split_y=y_minimum+floor((y_maximum-y_minimum)/2);
		node->split_point=split_y;
		node->left=Create_node(node->node_min_x,split_y,node->node_max_x,node->node_max_y,var);
		node->right=Create_node(node->node_min_x,node->node_min_y,node->node_max_x,split_y,var);
		Re_Order(node,id,x,y,var);
	}
}

void Insert(struct Node *node,vector< vector<int> > points, char var){
	for(int i=0;i<points.size();i++) {
		struct Node *tmp=node;
		int id=points[i][0];
		int x=points[i][1];
		int y=points[i][2];
		int dir=1;
		while(tmp->direction!=0){
			dir=tmp->direction;
			if(tmp->direction==1 &&  x<=tmp->split_point)   //horizontal split check median of 
				tmp=tmp->left;
			else if(tmp->direction==1 && x>tmp->split_point)
				tmp=tmp->right;
			else if(tmp->direction==2 && y>=tmp->split_point)
				tmp=tmp->left;
			else if(tmp->direction==2 && y<tmp->split_point)
				tmp=tmp->right;
		}


		int c=Counter(tmp->bucket_pointer,var);
		if(c==SIZE)	{
			split(tmp,id,x,y,dir,var);
		}
		else if(c<SIZE){
			struct Bucket *bucket=tmp->bucket_pointer;
			bucket->a[c][0]=id;
			bucket->a[c][1]=x;
			bucket->a[c][2]=y;
		}
	}
}


void Display(struct Node *node, char var){
	cout<<"direction : "<<node->direction<<"splitpoint : "<<node->split_point<<endl;
	cout<<"min_x :"<<node->node_min_x<<" min_y :"<<node->node_min_y<<" max_x :"<<node->node_max_x<<" max_y :"<<node->node_max_y<<endl;
	if(!node->direction){// == 0){
		struct Bucket *bucket = node->bucket_pointer;
		int c = Counter(bucket,var);
		for(int i=0;i<c;i++){
			cout<<bucket->a[i][0]<<" "<<bucket->a[i][1]<<" "<<bucket->a[i][2]<<" || ";
		}
		cout<<"------------------------"<<endl;
	}
}



//void findpoints(int min_x,int min_y,int max_x,int max_y,struct Node *node){
	
//}


void search(struct Node *node, int min_x,int min_y,int max_x,int max_y, char var){
	if(node!=NULL)
	{
	 if(node->direction==0){
			//findpoints(min_x,min_y,max_x,max_y,node);
			int c=Counter(node->bucket_pointer,var);
		for(int i=0;i<c;i++){
			if(node->bucket_pointer->a[i][1]>=min_x && node->bucket_pointer->a[i][1]<=max_x){
				if(node->bucket_pointer->a[i][2]>=min_y && node->bucket_pointer->a[i][2]<=max_y){
				cout<<setw(5)<<node->bucket_pointer->a[i][0]<<setw(12)<<node->bucket_pointer->a[i][1]<<setw(14)<<node->bucket_pointer->a[i][2]<<endl;
			}
			}
		}
	}

	else if(node->direction==1){
				int min=max_x<=node->split_point?max_x:node->split_point;
				search(node->left, min_x,min_y,min,max_y,var);
				min=min_x>=node->split_point?min_x:node->split_point;
				search(node->right, min,min_y,max_x,max_y,var);
		}
	else if(node->direction==2){
			int min=max_y<=node->split_point?max_y:node->split_point;
			search(node->right, min_x,min_y,max_x,min,var);
			min=min_y>=node->split_point?min_y:node->split_point;
			search(node->left, min_x,min,max_x,max_y,var);
		}
		
	}
}


int main(){
	vector<vector<int> > points;
	fstream file;
	int id,x,y,count=0;
	file.open("test.txt");
	while(!file.eof()){
		file>>id>>x>>y;
		vector<int> tmp;
		tmp.push_back(id);
		tmp.push_back(x);
		tmp.push_back(y);
		points.push_back(tmp);
		count++;
	}
	char var='t';
	points.pop_back();
	cout<<"Now inserting nodes"<<endl;
	Initialize(0,0,200,200,var);
	Insert(root,points,var);
	char Case='y';
	int a,b,c,d;
	do{
		cout<<"Enter minx miny maxx maxy:";
		cin>>a>>b>>c>>d;
		min_x=a;
		min_y=b;
		max_x=c;
		max_y=d;
		cout<<"\n*********************************************************\n";
		cout<<"NODE_ID"<<setw(15)<<"X-COORDINATE"<<setw(15)<<"Y-COORDINATE\n";
		cout<<"======="<<setw(15)<<"============"<<setw(15)<<"============\n\n";
		search(root, a,b,c,d,var);
		cout<<"\n***********************************************************\n";
		cout<<"Want more query y/n :\n";

		cout<<"Press 'y' to continue...\n";
		cout<<"Press 'n' to exit....\n";
		cout<<"***********************************************************\n";
		cin>>Case;
	}while(Case=='y');
	return 0;
}