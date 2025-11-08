// 11327132李晨希 11327141羅元廷
#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;

struct StackNode {
    int x, y;           // 位置座標
    int direction;      // 當前嘗試的方向 (0=右, 1=下, 2=左, 3=上)
    StackNode* next;
    
    StackNode(int _x, int _y) : x(_x), y(_y), direction(0), next(nullptr) {}
};
class Stack {
private:
    StackNode* top;
    
public:
    Stack() : top(nullptr) {}
    
    ~Stack() {
        while (!isEmpty()) {
            pop();
        }
    }
    
    void push(int x, int y) {
        StackNode* newNode = new StackNode(x, y);
        newNode->next = top;
        top = newNode;
    }
    
    void pop() {
        if (isEmpty()) return;
        StackNode* temp = top;
        top = top->next;
        delete temp;
    }
    
    StackNode* peek() {
        return top;
    }
    
    bool isEmpty() {
        return top == nullptr;
    }
    
    // 檢查某個位置是否已在堆疊中
    bool contains(int x, int y) {
        StackNode* current = top;
        while (current != nullptr) {
            if (current->x == x && current->y == y) {
                return true;
            }
            current = current->next;
        }
        return false;
    }
};
class Maze {
private:
    char** grid;        // 動態2D陣列
    int width, height;  // 寬度和高度
    
public:
    Maze() : grid(nullptr), width(0), height(0) {}
    
    ~Maze() {
        if (grid != nullptr) {
            for (int i = 0; i < height; i++) {
                delete[] grid[i];
            }
            delete[] grid;
        }
    }
    
    // 讀取迷宮檔案
    bool readFromFile(int fileNum) {
        char filename[20];
        sprintf(filename, "input%d.txt", fileNum);
        
        ifstream file(filename);
        if (!file.is_open()) {
            cout << "### " << filename << " does not exist! ###" << endl;
            return false;
        }
        
        // 讀取寬度和高度
        file >> width >> height;
        
        // 動態配置2D陣列
        grid = new char*[height];
        for (int i = 0; i < height; i++) {
            grid[i] = new char[width + 1];  // +1 給字串結束符
            file >> grid[i];
        }
        
        file.close();
        return true;
    }
    
    // 顯示迷宮
    void display() {
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                cout << grid[i][j];
            }
            cout << endl;
        }
    }
    
    // 取得某個位置的字符
    char getCell(int x, int y) {
        if (x < 0 || x >= width || y < 0 || y >= height) {
            return 'O';  // 邊界外視為障礙物
        }
        return grid[y][x];
    }
    
    // 設定某個位置的字符
    void setCell(int x, int y, char c) {
        if (x >= 0 && x < width && y >= 0 && y < height) {
            grid[y][x] = c;
        }
    }
    
    int getWidth() { return width; }
    int getHeight() { return height; }
};











void HandleInvalidInput(const string &message) {
  cout << message << endl;
  cin.clear();
  cin.ignore(numeric_limits<streamsize>::max(), '\n');
} // 清理緩衝區
void HandleCommand(int cmd) {
  switch (cmd) {
    case 1:
      
      break;
    case 2:
      
      break;
    case 3:
     
      break;
    case 4:
      
    case 0:
      break;
  }
}
void ShowMenu() {
  cout << "\n*** (^_^) Data Structure (^o^) ***\n";
  cout << "*** Find the Goal(s) in a Maze ***\n";
  cout << "* 0. Quit                        *\n";
  cout << "* 1. Find one goal               *\n";
  cout << "* 2. Find goal(s) as requested   *\n";
  cout << "* 3. How many goals?             *\n";
  cout << "* 4. Shortest path to one goal   *\n";
  cout << "**********************************\n";
  cout << "Input a command(0, 1, 2, 3, 4):  ";
}

int GetCommand() {
  if (cin.peek() == '\n') {
    cin.ignore();
    return 0;
  }
}
int main(int argc, const char * argv[]) {
  // insert code here...
  std::cout << "Hello, World!\n";
  return 0;
}


