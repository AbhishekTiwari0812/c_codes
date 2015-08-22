#include<iostream>
using namespace std;


int main(){

	int central;
	int temp1,temp2,temp3,temp4;
	int old_temp1,old_temp2,old_temp3,old_temp4;
	cin>>central>>old_temp1>>old_temp2>>old_temp3>>old_temp4;
	while(1==1){
		temp1=(old_temp1*3+central*2)/5;
		temp2=(old_temp2*3+central*2)/5;
		temp3=(old_temp3*3+central*2)/5;
		temp4=(old_temp4*3+central*2)/5;
		if(temp1==old_temp1 &&temp2==old_temp2 &&temp3==old_temp3 &&temp4==old_temp4 )
			break;
		else{
			old_temp1=temp1;
			old_temp2=temp2;
			old_temp3=temp3;
			old_temp4=temp4;
			central=(2*central+temp1+temp2+temp3+temp4)/6;
		}
		cout<<central<<" "<<temp1<<" "<<temp2<<" "<<temp3<<" "<<temp4<<endl;
	}
	cout<<central<<" "<<temp1<<" "<<temp2<<" "<<temp3<<" "<<temp4<<endl;
return 0;
}
