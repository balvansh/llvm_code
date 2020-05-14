int main(){
	int i, j, sum;
	sum=0;
	while(i!=10){
		if (i%2==0)
		{
			sum = sum+(4*i);
		} else{
			for(j=0; j<sum; j++){
				sum = sum +j;
			}
		}
		print("%d",sum);

	}
	return 0;
	
}
