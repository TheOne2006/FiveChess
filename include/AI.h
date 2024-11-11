// 这是一个虚拟的人机。
#include <board.h>
#include <vector>

// 使用决策树与a-b剪枝实现AI，需要注意的是其实不需要存储树状结构

// 返回下一步应该走的位置
point moveNext(board *b);