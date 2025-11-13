// 11327132李晨希 11327141羅元廷
#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;
void ShowMenu() {
  cout << "\n*** (^_^) Data Structure (^o^) ***\n";
  cout << "*** Find the Goal(s) in a Maze ***\n";
  cout << "* 0. Quit                        *\n";
  cout << "* 1. Find one goal               *\n";
  cout << "* 2. Find goal(s) as requested   *\n";
  cout << "* 3. How many goals?             *\n";
  cout << "* 4. Shortest path to one goal   *\n";
  cout << "**********************************\n";
  cout << "Input a command(0, 1, 2, 3, 4): ";
}
void HandleInvalidInput(const string &message) {
  cout << message << endl;
  cin.clear();
  cin.ignore(numeric_limits<streamsize>::max(), '\n');
}
int GetCommand() {
  if (cin.peek() == '\n') {
    cin.ignore();
  }
  
  int cmd;
  while (true) {
    
    if (cin >> cmd && cmd >= 0 && cmd <= 4) {
      char next = cin.peek();
      while (next == ' ' || next == '\t') {
        cin.get();
        next = cin.peek();
      }
      
      if (cin.peek() == '\n' || cin.peek() == EOF) {
        
        cin.ignore();
        return cmd;
      }
    }
    HandleInvalidInput("\nCommand does not exist!");
    ShowMenu();
  }
}
string getFileNumber() {
    string input;
    cout << "\nInput a file number: ";
    
    while (true) {
        getline(cin, input);
        
        // 去除前後空白
        size_t start = input.find_first_not_of(" \t\r\n");
        size_t end = input.find_last_not_of(" \t\r\n");
        
        if (start != string::npos) {
            input = input.substr(start, end - start + 1);
            return input;  // 有內容就回傳
        }
    }
}
int GetNumWithRange(int min, int max) {
  cout << "\n";
  cout << "Number of G (goals): ";
  int num = 0;
  while (!(cin >> num) || num < min || num > max) {
      if (cin.fail()) {
          // 清除 std::cin 的錯誤狀態
          cin.clear();
          // 忽略輸入緩衝區中的剩餘字元，直到換行符
          // streamsize 是資料型別,用來存放字元數量或資料流大小
          cin.ignore(numeric_limits<streamsize>::max(), '\n');
      } else {
          cout << "\n";
          cout << "### The number must be in [" << min << "," << max << "] ###" << endl;
      
      }
      cout << "\n";
    cout << "Number of G (goals): ";
  }
  return num;
}

struct StackNode {
  int x, y;
  int direction; // 這個點走過幾種方向
  int usedDir; // 用哪個方向到達這個點
  StackNode* next;
    
  StackNode(int _x, int _y) : x(_x), y(_y), direction(0), usedDir(0), next(nullptr) {} // 初始化
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
    bool readFromFile(const string& fileNum) {
      string filename = "input" + fileNum + ".txt";
        
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
                  grid[i] = new char[width + 1];
                  
                  string line;
                  file >> line;
                  strcpy(grid[i], line.c_str());
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
      cout << "\n";
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
  
    // 拷貝
    Maze* clone() const {
      Maze* copy = new Maze();
      copy->width = width;
      copy->height = height;

      copy->grid = new char*[height];
      for (int i = 0; i < height; i++) {
        copy->grid[i] = new char[width + 1];
        strcpy(copy->grid[i], grid[i]);
      }

      return copy;
    }
  
    bool isEmpty() const {
      return (width <= 0 || height <= 0);
  }
  
  int getWidth() const { return width; }
      int getHeight() const { return height; }
};

void markRoute(Stack& path, Maze& originalMaze, Maze* routeMaze) {
    StackNode* node = path.peek();
    
    while (node != nullptr) {
        int x = node->x;
        int y = node->y;
        
        // 目標位置保持 G，其他位置標記為 R
        if (originalMaze.getCell(x, y) == 'G') {
          routeMaze->setCell(x, y, 'G');
        } else {
          routeMaze->setCell(x, y, 'R');
        }
        node = node->next;
    }
}
void restoreGoals(Maze& originalMaze, Maze* visitedMaze) {
    for (int y = 0; y < originalMaze.getHeight(); y++) {
        for (int x = 0; x < originalMaze.getWidth(); x++) {
            if (originalMaze.getCell(x, y) == 'G' && visitedMaze->getCell(x, y) == 'V') {
                
              
                visitedMaze->setCell(x, y, 'G');
            }
        }
    }
}
int dx[] = {1, 0, -1, 0};  // 右, 下, 左, 上
int dy[] = {0, 1, 0, -1};

bool DFS_findGoals(Maze& originalMaze, Maze* visitedMaze, Maze* routeMaze, int targetGoals) {
    Stack path;
    int goalsFound = 0;
    
    // 將起點(0,0)加入Stack
    path.push(0, 0);
  
    while (!path.isEmpty()) {
        StackNode* top = path.peek();
        int x = top->x;
        int y = top->y;
        
        if (visitedMaze->getCell(x, y) == 'G') {
          visitedMaze->setCell(x, y, 'V');
          goalsFound++;
          if (goalsFound >= targetGoals) {
            restoreGoals(originalMaze, visitedMaze);
            markRoute(path, originalMaze, routeMaze);
              return true;
            }
        }
  
        if (visitedMaze->getCell(x, y) == 'E') {
            visitedMaze->setCell(x, y, 'V');
        }
        
        // 嘗試下一個方向
        bool moved = false;
      int prevUsedDir = (top->next != nullptr) ? top->next->usedDir : top->usedDir;
      
        while (top->direction < 4) {
          int dir = (prevUsedDir + top->direction) % 4;
          int next_x = x + dx[dir];
          int next_y = y + dy[dir];
          top->direction++;
            
            // 檢查是否可走 (O(1) 檢查 visitedMaze)
            char nextCell = visitedMaze->getCell(next_x, next_y);
            
            if (nextCell != 'V' && (nextCell == 'E' || nextCell == 'G')) {
              top->usedDir = dir;
                path.push(next_x, next_y);
                moved = true;
                break;
            }
        }
        
        if (!moved) {
            path.pop();
        }
    }
  
    restoreGoals(originalMaze, visitedMaze);
    return false;
}
int DFS_countAllGoals(Maze& originalMaze, Maze* visitedMaze) {
    Stack path;
    int goalsFound = 0;
    
    // 將起點(0,0)加入Stack
    path.push(0, 0);
  
    while (!path.isEmpty()) {
        StackNode* top = path.peek();
        int x = top->x;
        int y = top->y;
        
        if (visitedMaze->getCell(x, y) == 'G') {
          visitedMaze->setCell(x, y, 'V');
          goalsFound++;
        }
  
        if (visitedMaze->getCell(x, y) == 'E') {
            visitedMaze->setCell(x, y, 'V');
        }
        
        // 嘗試下一個方向
        bool moved = false;
        
      int prevUsedDir = (top->next != nullptr) ? top->next->usedDir : top->usedDir;
      
       
      
        while (top->direction < 4) {
          int dir = (prevUsedDir + top->direction) % 4;
          int next_x = x + dx[dir];
          int next_y = y + dy[dir];
            top->direction++;
            
            // 檢查是否可走 (O(1) 檢查 visitedMaze)
            char nextCell = visitedMaze->getCell(next_x, next_y);
            
            if (nextCell != 'V' && (nextCell == 'E' || nextCell == 'G')) {
              top->usedDir = dir;
                path.push(next_x, next_y);
                moved = true;
                break;
            }
        }
        
        if (!moved) {
            path.pop();
        }
    }
  
    restoreGoals(originalMaze, visitedMaze);
    return goalsFound;
}

void Task1(Maze& originalMaze) {
  Maze* visitedMaze = originalMaze.clone(); // 複製originalMaze給visitedMaze
  Maze* routeMaze = originalMaze.clone(); // 複製originalMaze給routeMaze

  if (DFS_findGoals(originalMaze, visitedMaze, routeMaze, 1)) {
    visitedMaze->display();
    routeMaze->display();
  } else {
    visitedMaze->display();
  }
  
  delete visitedMaze;
  delete routeMaze;
}

void Task2(Maze& originalMaze, int targetGoals) {
  Maze* visitedMaze = originalMaze.clone(); // 複製originalMaze給visitedMaze
  Maze* routeMaze = originalMaze.clone(); // 複製originalMaze給routeMaze

  if (DFS_findGoals(originalMaze, visitedMaze, routeMaze, targetGoals)) {
    visitedMaze->display();
    routeMaze->display();
  } else {
    visitedMaze->display();
  }
  
  delete visitedMaze;
  delete routeMaze;
}

void Task3(Maze& originalMaze) {
  Maze* visitedMaze = originalMaze.clone(); // 複製originalMaze給visitedMaze

  int goalsFound = DFS_countAllGoals(originalMaze, visitedMaze);
  visitedMaze->display();
  cout << "The maze has " << goalsFound << " goal(s) in total." << endl;
  
  delete visitedMaze;
}

void HandleCommand(int cmd, Maze& originalMaze) {
  switch (cmd) {
    case 1: {
      string filenum = getFileNumber();
      bool readSuccess = originalMaze.readFromFile(filenum);
      if (readSuccess) {
        Task1(originalMaze);
      }
      break;
    }
    case 2: {
      if (originalMaze.isEmpty()) {
        cout << "\n### Execute command 1 to load a maze! ###" << endl;
        break;
      }
      int targetGoals = GetNumWithRange(1, 100);
      Task2(originalMaze, targetGoals);
      break;
    }
    case 3: {
      if (originalMaze.isEmpty()) {
        cout << "\n### Execute command 1 to load a maze! ###" << endl;
        break;
      }
      Task3(originalMaze);
      break;
    }
    case 0:
      break;
  }
}


int main() {
  Maze original;
  int cmd;
  do {
    ShowMenu();
    cmd = GetCommand();
    HandleCommand(cmd, original);
  } while (cmd != 0);
  return 0;
}


