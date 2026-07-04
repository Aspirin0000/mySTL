<p align="center">
  <img src="https://img.shields.io/badge/C%2B%2B-17-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white" />
  <img src="https://img.shields.io/badge/Containers-STL%20Subset-0EA5E9?style=for-the-badge" />
  <img src="https://img.shields.io/badge/License-MIT-yellow?style=for-the-badge" />
  <img src="https://img.shields.io/badge/Header%20Only-Yes-22c55e?style=for-the-badge" />
</p>

# mySTL

**课程型 STL 复现仓库**，实现常见标准容器的关键语义和复杂度行为，强调可读性、接口一致性和算法意识。

## 目录
- [项目目标](#项目目标)
- [已实现容器](#已实现容器)
- [复杂度速览](#复杂度速览)
- [目录结构](#目录结构)
- [最小验证示例](#最小验证示例)
- [整理记录](#整理记录)
- [后续迭代](#后续迭代)
- [许可证](#许可证)

## 项目目标

- 从零理解容器实现中的边界：迭代器、内存管理、拷贝语义、异常安全。
- 使用统一风格组织 `hpp` 实现，保持和 STL 使用习惯一致。
- 为课程提交与知识复盘提供稳定示例仓库。

## 已实现容器

| 容器 | 能力 | 复杂度 | 学习收益 |
|---|---|---|---|
| `vector` | 随机访问、push/pop、迭代器、动态扩容 | 均摊 O(1) / O(1) | 内存局部性与容量策略 |
| `list` | 双向链表、插入删除、双向遍历 | O(1) | 指针链接与节点边界 |
| `priority_queue` | 堆操作、入堆出堆、比较器 | O(log n) | 堆与优先级调度 |
| `map` | 红黑树实现、有序键值存取 | O(log n) | 平衡树插入/查找/删除 |

## 复杂度速览

```text
vector     : 快速随机访问 + 连续内存
list       : 低开销插入删除 + 非连续内存
priority_queue : 顶部访问优先级队列
map        : 稳定有序遍历 + 受限查找成本
```

## 目录结构

```text
├─ vector.hpp
├─ list.hpp
├─ map.hpp
├─ priority_queue.hpp
├─ utility/
│  ├─ algorithm.hpp
│  ├─ exceptions.hpp
│  └─ utility.hpp
└─ LICENSE
```

## 最小验证示例

```cpp
#include "vector.hpp"
#include "map.hpp"

int main() {
  my_stl::vector<int> nums{1, 2, 3};
  my_stl::map<int, int> score;
  score[1] = 10;
  return nums.size() == 3 && score.size() == 1 ? 0 : 1;
}
```

```bash
g++ -std=c++17 -I. -O2 -o example example.cpp
./example
```

## 整理记录

- 已清理：课程阶段的噪音文件与非核心临时文件
- 保留：核心容器实现、实用工具与许可证文件

## 后续迭代

- [ ] 补齐迭代器可诊断能力与常见异常分支测试
- [ ] 引入独立测试框架与 CI 覆盖
- [ ] 增加文档化的接口行为说明（复杂度与边界）

## 许可证

MIT —— 详见 `LICENSE`。
