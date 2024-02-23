#include <vector>
#include <algorithm>
using namespace std;

bool greater2(int x1, int x2) {return x1 > x2;}

int main()
{
    vector<int> v;
    ....// 往容器中放元素
    for_each(v.begin(), v.end(), f); // 对v中的每个元素去调用函数f进行操作
    return 0;
}