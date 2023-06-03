//引入需要使用的几个头文件
#include<graphics.h>
#include<time.h> 
#include<iostream>
#include<windows.h>
#include <cstdlib>
using namespace std;
const int gz_size = 25;//格子常量
class MineSweeper { //扫雷类
private:
	int **M;//存储周围雷个数
	int **flag;//格子状态
	int row, col, num; //游戏中方块的行数、列数以及地雷数，由于窗口大小限制，行数不超过40行，列数不超过76行，故雷数不超过3040个
	int sign_num;//待标记雷数
	bool is_over;  //定义判断游戏是否结束的bool量，默认为false（未结束）
	bool is_win;  //定义判断游戏结束是否通关的bool量，默认为false（未通关）
	bool first_click ; // 是否是第一次点击
	int count;//已经打开的格子数（不包括雷），以此判断通关与否
	double start_time;
	double now_time; 
	//定义 雷，踩雷，数字（0-8），格子，空格，旗，笑脸，哭脸，墨镜脸对象
	//以及雷数目和计时对象（0-9）
	PIMAGE img_mine;//普通雷
	PIMAGE img_gz;//未打开的格子
	PIMAGE img_number[9];//格中数字
	PIMAGE img_boom;//踩雷
	PIMAGE number[10];//雷数和计时数字
	PIMAGE smiley_face;//笑脸
	PIMAGE cry_face;//哭脸
	PIMAGE glasses_face;//墨镜脸
	PIMAGE mine_flag;//旗子
	
	mouse_msg msg;   //鼠标类对象（获取左右键的状态和坐标值）
	
public:
	MineSweeper(); //构造函数，进行游戏初始化(随机布雷、计算方块数字、随机寻找空白方块并打开、如果开启自动标记则进行自动标记)
	~MineSweeper();//析构函数，数据成员中有内存的动态分配，因此需要专门的析构函数
	void initGame();//初始化游戏界面
	void Open(const int&, const int&); //打开选中方块，若打开空白方块则自动打开周围方块；取消标记；若被标记则取消标记
	void Sign(const int&, const int&); //标记选中方块；取消标记；
	void run();//游戏运行函数
	void Display();//显示所有图片，用于检查调试
	void click();//获取鼠标左右键点击状态及坐标，调用open函数
	void click_first();//点一次点击函数，将第一次鼠标左键点击的坐标传给Generate函数进行随机生成雷
	void Calc();//计算当前格子周围3*3范围的雷数
	void Generate(int ,int );//随机生成雷
	void Show();//在控制台显示雷（*），数字（1-8），空白格（ ）
	void input();//输入参数
	int  is_finish();//判断是否通关or踩雷
	void Restgame();//重置游戏的相关参数
	void message(int k,int x,int y);//执行通关or踩雷的相关操作
	void time_display();//计时显示
};
MineSweeper::MineSweeper() {//构造函数
	//初始化窗口
	input();
	initgraph(col* gz_size, (row + 1)*gz_size);
	setcaption("扫雷");
	setbkcolor(RGB(204,204,204) );
	sign_num=num;
	count=0;
	first_click = true;//第一次点击
	is_over = false; //定义游戏默认为false（未结束）
	is_win = false; //定义通关状态默认为false（未通关）
	//为数据成员二重M整形指针动态分配内存,全部初始化为0（非雷）
	// 动态分配行
	M = new int *[row];
	// 动态分配列
	for (int i = 0; i < row; i++) {
		M[i] = new int[col];
	}
	
	//为数据成员二重flag整形指针动态分配内存,全部初始化为0（非雷）
	// 动态分配行
	flag = new int *[row];
	
	// 动态分配列
	for (int i = 0; i < row + 1; i++) {
		flag[i] = new int[col];
	}
	
	for (int i = 0; i < row + 1; i++) {
		for (int j = 0; j < col; j++) {
			if (i<1) {
				flag[i][j] = 1; //第一行设为不可打开状态
			} else {
				flag[i][j] = 0; //其余行为未打开状态
			}
		}
	}
	//	Show();控制台显示雷是否正常
	//雷的素材
	img_mine = newimage();
	getimage(img_mine, "pic/雷.png");
	
	
	//周围雷数目数字的素材
	for (int i = 0; i <= 8; i++) {
		img_number[i] = newimage();
		char buf[100];
		sprintf(buf, "pic/雷%d.png", i);
		getimage(img_number[i], buf);
	
	}
	
	//格子的素材
	img_gz = newimage();
	getimage(img_gz, "pic/格子.png");
	
	//踩雷素材
	img_boom = newimage();
	getimage(img_boom, "pic/踩雷.png");
	
	
	//雷数和计时素材
	for (int i = 0; i <= 9; i++) {
		number[i] = newimage();
		char buf[100];
		sprintf(buf, "pic/%d.png", i);
		getimage(number[i], buf);
	
	}
	
	//笑脸素材
	smiley_face = newimage();
	getimage(smiley_face, "pic/笑脸.png");
	
	
	//哭脸素材
	cry_face = newimage();
	getimage(cry_face, "pic/哭脸.png");
	
	
	//墨镜脸素材
	glasses_face = newimage();
	getimage(glasses_face, "pic/墨镜脸.png");
	
	
	//旗的素材
	mine_flag = newimage();
	getimage(mine_flag, "pic/旗.png");

	
}
MineSweeper::~MineSweeper() {//析构函数
	//MineSweeper对象生命周期结束后delete动态分配的内存
	for (int i = 0; i < row + 1; i++)
		if (flag[i])
			delete[] flag[i];
	if (flag)
		delete[] flag;
	
	for (int i = 0; i < row; i++)
		if (M[i])
			delete[] M[i];
	if (M)
		delete[] M;
	
	closegraph();
}
void MineSweeper::Generate(int x,int y){//随机布雷函数	
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			M[i][j] = 0; //全部设为非雷0
		}
	}
	srand(time(NULL));  // 设定随机种子
	int cnt = 0;
	while (cnt < num) {
		int rw = rand() % row;
		int cl = rand() % col;
		if (M[rw][cl] == 0) {  // 如果当前位置没有设置为雷且不是第一次点击的位置
			if(rw!=y&&cl!=x){
				M[rw][cl] = 9;  // 将该位置设置为雷
				cnt++;
			}
			
		}
	}
	Calc();//计算雷
	cout<<endl<<endl<<endl;
	Show();//控制台显示计算是否正常
	cout<<endl<<endl<<endl;
	
}
void MineSweeper:: Calc(){//计算周围雷数函数
	for(int j=0;j<row;j++){
		for(int i=0;i<col;i++){
			if(M[j][i]==9)
				continue;
			int number=0;
			for(int n=-1;n<=1;n++){
				int y=j+n;
				if(y<0||y>=row) continue;
				for(int m=-1;m<=1;m++){
					int x=m+i;
					if(x<0||x>=col) continue;
					if(M[y][x]==9) 
						number++;
				}
			}
			M[j][i]=number;
		}	
	}
}
void MineSweeper::Show(){//控制台显示雷情况函数，用以调试
	for (int y = 0; y <row; y++) {
		for (int x = 0; x < col; x++) {
			if (M[y][x] == 9)
				cout << "* ";
			else {
				if (M[y][x] == 0)
					cout << "  ";
				else {
					cout << M[y][x] << " ";
				}
			}
		}
		cout << endl;
	}
}
void MineSweeper::Display(){//将所有图片贴出函数，用以调试
	for (int y = 0; y < row; y++) {
		for (int x = 0; x < col; x++) {
			if (M[y][x] == 9)
				putimage(x * 25, y * 25 + 25, img_mine);
			else
				putimage(x * 25, y * 25 + 25, img_number[M[y][x]]);
		}
	}
	getch();
	
} 
void MineSweeper::initGame(){//初始化游戏界面函数
	for(int i=0;i<row;i++){
		for(int j=0;j<col;j++){
			putimage(j*25,i*25+25,img_gz);
		} 
	} 
	if(sign_num<1000){
		putimage(0, 1, number[sign_num/100]); //雷数的第一个数字
		putimage(13, 1, number[sign_num/10%10]); //雷数的第二个数字
		putimage(26, 1, number[sign_num%10]); //雷数的第三个数字
	}else if(sign_num>=1000){
		putimage(0, 1, number[sign_num/1000]); //雷数的第一个数字
		putimage(13, 1, number[sign_num/100%10]); //雷数的第二个数字
		putimage(26, 1, number[sign_num/10%10]); //雷数的第三个数字
		putimage(39, 1, number[sign_num%10]); //雷数的第三个数字
	}
	
	putimage(col*gz_size/2-12, 0, smiley_face); //笑脸
	putimage(col*gz_size-13, 1, number[0]); //计时的第一个数字
	putimage(col*gz_size-26, 1, number[0]); //计时的第二个数字
	putimage(col*gz_size-39, 1, number[0]); //计时的第三个数字
}
void MineSweeper::click(){//左右键点击函数
	setrendermode(RENDER_MANUAL); //不使用自动刷新，减少闪烁 
	mouse_msg msg = {0};
	for ( ; is_run(); delay_fps(60))
	{
		//获取鼠标消息，此函数不会等待，运行后会立即返回
		while (mousemsg())
		{
			msg = getmouse();
			if(msg.is_left()&&msg.is_down()){
				Open(msg.x/25,msg.y/25);
				if(msg.y/25==0&&msg.x>=col*25/2-12&&msg.x<col*25/2+13){//重新开始游戏
					Restgame();
				}
			}
			if(msg.is_right()&&msg.is_down()){
				Sign(msg.x/25,msg.y/25);
			}
		}
		time_display();
	}	
}
void MineSweeper:: click_first(){//左键第一次点击函数
	setrendermode(RENDER_MANUAL); //不使用自动刷新，减少闪烁 
	mouse_msg msg = {0};
	for ( ; is_run()&&first_click; delay_fps(60)){
		//获取鼠标消息，此函数不会等待，运行后会立即返回
		while (mousemsg())
		{
			msg = getmouse();
			if(msg.is_left()&&msg.is_down()){
				if(msg.y>=25&&msg.y<=(row+1)*25&&msg.x>=0&&msg.x<=col*25){
					Generate(msg.x/25,msg.y/25);
					Open(msg.x/25,msg.y/25);
					start_time=fclock();
					first_click=false;
				}
			}
			
		}
		
	}
	time_display();
	click();
	
}
void MineSweeper::Open(const int &x, const int &y){//打开格子函数
	// 添加判断语句，如果点击的是地图范围内的位置
	if (!((y < 1 || y > row) || (x < 0 || x >= col))) {
		if (flag[y][x] == -1) {
			putimage(x * 25, y * 25, img_gz); // 撤销旗子，恢复格子
			flag[y][x] = 0;
			sign_num++;
			putimage(0, 1, number[sign_num/100]); //雷数的第一个数字
			putimage(13, 1, number[sign_num/10%10]); //雷数的第二个数字
			putimage(26, 1, number[sign_num%10]); //雷数的第三个数字
			return ;
		}
		else if (flag[y][x] == 0) {
			flag[y][x] = 1;
			if (M[y - 1][x] == 9) {
				putimage(x * 25, y * 25, img_boom);
				putimage(col*25/2-12,0,cry_face);//哭脸
				is_over = true;	
				message(-1,x,y);
			} else if (M[y - 1][x] != 0) {
				count++;
				putimage(x * 25, y * 25, img_number[M[y - 1][x]]);
				if (count > row * col - num - 1) {//判断是否通关
					putimage(col*25/2-12,0,glasses_face);
					is_over = true;
					is_win = true;
					message(1,x,y);
				
				}
				
				return ;
			}else{
				count++;
				putimage(x * 25, y * 25, img_number[0]);
				// 种子填充算法
				if(!((y < 1 || y > row) || (x -1< 0 || x-1 > col))){
					if(M[y-1][x-1]!=9){
						Open(x - 1, y);
					}else
						return;
				}
				if(!((y -1< 1 || y-1 > row) || (x -1< 0 || x-1 >col))){
					if(M[y-2][x-1]!=9){
						Open(x - 1, y - 1);
					}else
						return;
				}
				
				if(!((y -1< 1 || y-1 > row) || (x < 0 || x>col))){
					if(M[y-2][x-1]!=9){
						Open(x, y - 1);
					}else
						return ;
				}
				
				if(!((y -1< 1 || y-1 > row) || (x +1< 0 || x+1 >col))){
					if(M[y-2][x+1]!=9){
						Open(x + 1, y - 1);
					}else
						return;
				}
				
				if(!((y < 1 || y > row) || (x +1< 0 || x+1 >col))){
					if(M[y-1][x]!=9){
						Open(x + 1, y);
					}else
						return ;
				}
				
				if(!((y +1< 1 || y+1 > row) || (x +1< 0 || x+1 >col))){
					if(M[y][x+1]!=9){
						Open(x + 1, y + 1);
					}else
						return ;
				}
				
				if(!((y +1< 1 || y+1 > row) || (x < 0 || x >col))){
					if(M[y][x]!=9){
						Open(x, y + 1);
					}else
						return ;
				}
				if(!((y +1< 1 || y+1 > row) || (x -1< 0 || x-1 >col))){
					if(M[y][x-1]!=9){
						Open(x - 1, y + 1);
					}else
						return ;
				}
				
				//同样需要判断一次是否通关
				if (count > row * col - num - 1) {
					putimage(col*25/2-12,0,glasses_face);
					is_over = true;   //游戏状态结束
					is_win = true;    //通关
					message(1,x,y);
				}
			}
		}
		else
			return;
	}else if(y==0&&x*25>=(col*25/2-12)&&x<=col*25/2+13){
		delay(100);
		putimage(col*25/2-12,0,smiley_face);
		is_over=false;
		is_win=false;	
	}
}
void MineSweeper::Sign(const int &x,const int &y){//标雷与取消函数
	if (!((y < 1 || y > row) || (x < 0 || x > col))){//在窗口第一行之后
		if(flag[y][x]!=1){
			if(flag[y][x]==0){//若未标记则插上旗子
				if(sign_num){
					putimage(x*25,y*25,mine_flag);
					sign_num--;//待标记雷数减少
				}
				
				putimage(0, 1, number[sign_num/100]); //雷数的第一个数字
				putimage(13, 1, number[sign_num/10%10]); //雷数的第二个数字
				putimage(26, 1, number[sign_num%10]); //雷数的第三个数字
				flag[y][x]=-1;//记为已标记
				int k=0;
				for (int j = 0; j < row; j++) {
					for (int i = 0; i < col; i++) {
						if(flag[j+1][i]==-1&&M[j][i]==9)
							k++;
					}
				}
				if(sign_num==0&&k==num){
					putimage(x*25,y*25,mine_flag);
					if (count > row * col - num - 1) {//判断是否通关
					
						putimage(col*25/2-12,0,glasses_face);
					
						message(1,x,y);
						
					}
				}
				
			}
			else{//已标记则去除标记
				putimage(x*25,y*25,img_gz);
				sign_num++;//待标记雷数增加
				putimage(0, 1, number[sign_num/100]); //雷数的第一个数字
				putimage(13, 1, number[sign_num/10%10]); //雷数的第二个数字
				putimage(26, 1, number[sign_num%10]); //雷数的第三个数字
				flag[y][x]=0;//未打开状态
			}
		}
	}
	
}
void MineSweeper:: input(){//初始化参数函数
	int rw,cl,nm;
	cout<<"请依次输入行数(为正数且<=40)，列数(为正数且<=76)，雷数（为正数且<=3040）"<<endl;
	cin>>rw>>cl>>nm;
	while(1){
		if((rw>0&&rw<=40)&&(cl>0&&cl<=76)&&(nm>0&&nm<=3040))
			break;
		else{
			cout<<"参数错误请重新输入"<<endl;
			cin>>rw>>cl>>nm;
		}
		
	}
	
	row=rw;
	col=cl;
	num=nm;
	cout<<"***********游戏开始**********"<<endl;
	
}
void MineSweeper:: run(){//游戏执行函数
	initGame();
	click_first();	
}
int  MineSweeper::is_finish(){//判断是否通关函数
	if(is_win&&is_over){
		
		return 1;
	}
	else if(is_over&&!is_win){
		return -1;
	}
	return 0;
}
void MineSweeper::Restgame(){//重置游戏相关参数函数
	is_over=false;
	is_win=false;
	first_click=true;
	count=0;
	start_time=0;
	now_time=0;
	sign_num=num;
	for (int i = 0; i < row + 1; i++) {
		for (int j = 0; j < col; j++) {
			if (i<1) {
				flag[i][j] = 1; //第一行设为不可打开状态
			} else {
				flag[i][j] = 0; //其余行为未打开状态
			}
		}
	}
	cout<<"**********重新开始游戏**********"<<endl<<endl<<endl;
	initGame();
	click_first();
	Show();
}
void MineSweeper:: time_display() {//计时显示函数
	
	if(is_finish()==0){
		now_time = fclock();
		int elapsed_time = (int )now_time - start_time;
		if (elapsed_time <=999) {
			putimage(col * gz_size - 13, 1, number[elapsed_time % 10]);
			putimage(col * gz_size - 26, 1, number[(elapsed_time / 10) % 10]);
			putimage(col * gz_size - 39, 1, number[(elapsed_time / 100) % 10]);
		}
		else{
			putimage(col * gz_size - 13, 1, number[elapsed_time % 10]);
			putimage(col * gz_size - 26, 1, number[(elapsed_time / 10) % 10]);
			putimage(col * gz_size - 39, 1, number[(elapsed_time / 100) % 10]);
			putimage(col * gz_size - 52, 1, number[(elapsed_time / 1000) %10]);
		}
		
	}else{
		int elapsed_time = (int )now_time - start_time;
		if (elapsed_time <=999) {
			putimage(col * gz_size - 13, 1, number[elapsed_time % 10]);
			putimage(col * gz_size - 26, 1, number[(elapsed_time / 10) % 10]);
			putimage(col * gz_size - 39, 1, number[(elapsed_time / 100) % 10]);
			
		}
			
		else{
			putimage(col * gz_size - 13, 1, number[elapsed_time % 10]);
			putimage(col * gz_size - 26, 1, number[(elapsed_time / 10) % 10]);
			putimage(col * gz_size - 39, 1, number[(elapsed_time / 100) % 10]);
			putimage(col * gz_size - 52, 1, number[(elapsed_time / 1000)%10 ]);
		}
		
	}
	
}
void MineSweeper::message(int k,int x,int y){//通关or踩雷后若干操作函数
	
	for (int i = 0; i < row + 1; i++) {
		for (int j = 0; j < col; j++) {
			flag[i][j] = 1; 
	}
	}
	//click();
	
	if(k==1){
	
		
		putimage(col*25/2-12,0,glasses_face);
		
		for (int j = 0; j < row; j++) {
			for (int i = 0; i < col; i++) {
				if (M[j][i] == 9)
					putimage(i * 25, j * 25 + 25, mine_flag);//显示雷的位置
			}
		}
	 
	  cout<<"**********泰酷辣！恭喜通关!**********"<<endl<<endl<<endl;
	
		
	}else if(k==-1){
		putimage(x * 25, y * 25, img_boom);
		putimage(col*25/2-12,0,cry_face);//哭脸
		
			cout<<"**********可惜辣！踩到雷了！**********\n**********点击哭脸重新开始吧！**********"<<endl<<endl<<endl;
		
	}
}
//main函数
int main(){
	MineSweeper game;
	game.run();
	if(game.is_finish()==0)
		game.Restgame();
	return 0;
}
