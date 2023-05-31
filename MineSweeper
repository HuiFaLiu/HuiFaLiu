//引入需要使用的几个头文件
#include<graphics.h>
#include<time.h> //时间头文件 
#include<iostream>
#include <ege/sys_edit.h>//sys_edit类头文件:屏幕输入框

using namespace std;

const int gz_size = 25;  //定义格子大小


class MineSweeper { //扫雷类
private:
	int **M;//存储周围雷个数
	int **flag;//格子状态
	int row, col, num; //游戏中格子的行数、列数以及地雷数，由于窗口大小限制，行数不超过40行，列数不超过76行，故雷数不超过3040个
	int sign_num;//待标记雷数
	bool is_auto;//判断是否开启自动标记
	bool is_over;  //定义判断游戏是否结束的bool量，默认为false（未结束）
	bool is_win;  //定义判断游戏结束是否通关的bool量，默认为false（未通关）
	bool first_click ; // 是否是第一次点击
	static int count;//已经打开的格子数（不包括雷），以此判断通关与否
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
	//POINT first;
	MineSweeper(); //构造函数，进行游戏初始化(随机布雷、计算方块数字、随机寻找空白方块并打开、如果开启自动标记则进行自动标记)
	~MineSweeper();//析构函数，数据成员中有内存的动态分配，因此需要专门的析构函数
	void initGame();//初始化游戏界面
	//	void AutoSign();//自动标雷，如果已打开方块数字等于周围未打开方块数时，自动对其标记
	void Open(const int&, const int&); //打开选中方块，若打开空白方块则自动打开周围方块；取消标记；点击数字时若周围标记数大于等于数字，则打开周围status==0的方块，返回值用来判断是否闪烁(见status==-2)
	void Sign(const int&, const int&); //标记选中方块；取消标记；点击数字时与Open()相同
	//	void Loop();//单局游戏主循环
	void run();//游戏运行函数
	void Display(); //调用后显示所有位置对应的图片
	void click();//点击函数，获取鼠标左右键点击时的坐标
	void click_first();//获取鼠标左键第一次点击时的坐标
	void Calc();//计算非雷格子周围的雷数，将结果存储在二维M数组里
	void Generate(int ,int );//自动种雷
	void Show();//在控制台显示检查计算是否正确，为雷打印*，数字（1-8）打印数字，0打印空格
	void input();//输入行数，列数，雷数进行难度选择
};
//静态数据成员类外初始化
int MineSweeper::count = 0;

/*
 *构造函数
 */
MineSweeper::MineSweeper() {
	//初始化窗口
	input();
	initgraph(col* gz_size, (row + 1)*gz_size);
	setbkcolor(RGB(204,204,204) );
	sign_num=num;
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
	//	Show();
	//雷的素材
	img_mine = newimage();
	getimage(img_mine, "pic/雷.png");
	//	putimage(0,0,img_mine);
	
	//周围雷数目数字的素材
	for (int i = 0; i <= 8; i++) {
		img_number[i] = newimage();
		char buf[100];
		sprintf(buf, "pic/雷%d.png", i);
		getimage(img_number[i], buf);
		//putimage(i*25,25,img_number[i]);
	}
	
	//格子的素材
	img_gz = newimage();
	getimage(img_gz, "pic/格子.png");
	
	//踩雷素材
	img_boom = newimage();
	getimage(img_boom, "pic/踩雷.png");
	//putimage(0,0,img_gz);
	
	//雷数和计时素材
	for (int i = 0; i <= 9; i++) {
		number[i] = newimage();
		char buf[100];
		sprintf(buf, "pic/%d.png", i);
		getimage(number[i], buf);
		//putimage(i*13,23,number[i]);
	}
	
	//笑脸素材
	smiley_face = newimage();
	getimage(smiley_face, "pic/笑脸.png");
	//putimage(0,0,smiley_face);
	
	//哭脸素材
	cry_face = newimage();
	getimage(cry_face, "pic/哭脸.png");
	//putimage(25,0,cry_face);
	
	//墨镜脸素材
	glasses_face = newimage();
	getimage(glasses_face, "pic/墨镜脸.png");
	//putimage(50,0,glasses_face);
	
	//旗的素材
	mine_flag = newimage();
	getimage(mine_flag, "pic/旗.png");
	//putimage(0,0,ming_flag);
	
	
	//	if (is_auto) //判断是否自动标记
	//AutoSign();//进行自动标记
	
	
}



/*
 *析构函数
 */
MineSweeper::~MineSweeper() {
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



void MineSweeper::Generate(int x,int y){	
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
	//检查雷生成是否正常
	for (int y = 0; y <row; y++) {
		for (int x = 0; x < col; x++) {
			cout << M[y][x] << " ";
		}
		cout << endl;
	}
	
	Calc();//计算雷
	Show();//控制台显示计算是否正常
}

void MineSweeper:: Calc(){
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


void MineSweeper::Show(){
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


void MineSweeper::Display(){
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



void MineSweeper::initGame(){
	for(int i=0;i<row;i++){
		for(int j=0;j<col;j++){
			putimage(j*25,i*25+25,img_gz);
		} 
	} 
	putimage(0, 1, number[sign_num/100]); //雷数的第一个数字
	putimage(13, 1, number[sign_num/10%10]); //雷数的第二个数字
	putimage(26, 1, number[sign_num%10]); //雷数的第三个数字
	putimage(col*gz_size/2-12, 0, smiley_face); //笑脸
	putimage(col*gz_size-13, 1, number[0]); //计时的第一个数字
	putimage(col*gz_size-26, 1, number[0]); //计时的第二个数字
	putimage(col*gz_size-39, 1, number[0]); //计时的第三个数字
}

void MineSweeper::click(){
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
			}
			if(msg.is_right()&&msg.is_down()){
				Sign(msg.x/25,msg.y/25);
			}
		}
	}	
}


void MineSweeper:: click_first(){
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
							first_click=false;
					}
			}
				//break;
		}
	}
	click();
	
}



void MineSweeper::Open(const int &x, const int &y)
{
	// 添加判断语句，如果点击的是地图范围内的位置
	if (!((y < 1 || y > row) || (x < 0 || x > col))) {
		if (flag[y][x] == -1) {
			putimage(x * 25, y * 25, img_gz); // 撤销旗子，恢复格子
			flag[y][x] = 0;
			return;
		}
		if (flag[y][x] == 0) {
			flag[y][x] = 1;
			if (M[y - 1][x] == 9) {
				putimage(x * 25, y * 25, img_boom);
				for (int j = 0; j < row; j++) {
					for (int i = 0; i < col; i++) {
						if (j == y - 1 && i == x)
							continue;
						flag[j + 1][i] = 1;
						if (M[j][i] == 9) {
							putimage(i * 25, j * 25 + 25, img_mine);
						}
					}
				}
				is_over = true;
			} else if (M[y - 1][x] != 0) {
				count++;
				cout<<count<<endl;
				putimage(x * 25, y * 25, img_number[M[y - 1][x]]);
				if (count > row * col - num - 1) {
					for (int j = 0; j < row; j++) {
						for (int i = 0; i < col; i++) {
							flag[j + 1][i] = 1;
							if (M[j][i] == 9)
								putimage(i * 25, j * 25 + 25, img_mine);
						}
					}
					is_over = true;
					is_win = true;
				}
				
				return ;
			} else {
				count++;
				cout<<count<<endl;
				putimage(x * 25, y * 25, img_number[0]);
				flag[y][x] = 1;
				// 对周围格子进行判断，如果不是雷和已经打开的空白格，继续递归打开
				Open(x - 1, y);
				Open(x - 1, y - 1);
				Open(x, y - 1);
				Open(x + 1, y - 1);
				Open(x + 1, y);
				Open(x + 1, y + 1);
				Open(x, y + 1);
				Open(x - 1, y + 1);
				//同样需要判断一次是否通关
				if (count > row * col - num - 1) {
					for (int j = 0; j < row; j++) {
						for (int i = 0; i < col; i++) {
							flag[j + 1][i] = 1;
							if (M[j][i] == 9)
								putimage(i * 25, j * 25 + 25, img_mine);
						}
					}
					is_over = true;   //游戏状态结束
					is_win = true;    //通关
				}
			}
		}
	}
}




void MineSweeper::Sign(const int &x,const int &y){
	if (!((y < 1 || y > row) || (x < 0 || x > col))){//在窗口第一行之后
		if(flag[y][x]!=1){
			if(flag[y][x]==0){//若未标记则插上旗子
				putimage(x*25,y*25,mine_flag);
				sign_num--;//待标记雷数减少
				flag[y][x]=-1;//记为已标记
				//cout<<flag[y][x]<<"******"<<endl;
			}
			else{//已标记则去除标记
				putimage(x*25,y*25,img_gz);
				sign_num++;//待标记雷数增加
				flag[y][x]=0;//未打开状态
			}
		}
	}
	
}



void MineSweeper:: input(){
	int rw,cl,nm;
	cout<<"请依次输入行数，列数，雷数"<<endl;
	cin>>rw>>cl>>nm;
	row=rw;
	col=cl;
	num=nm;
}



void MineSweeper:: run(){
	while(!is_over&&!is_win){
		initGame();
		click_first();
		getch();
		//Display();
	}
	
	if(is_win){  //win 
		;
	}
	if(is_over&&!is_win){  //lose
		
	}
	
	closegraph();
}

int main(){
	MineSweeper game;
	game.run();
	return 0;
}
