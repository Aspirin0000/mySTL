<p align="center">
  <img src="https://img.shields.io/badge/C%2B%2B-17-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white" />
  <img src="https://img.shields.io/badge/Containers-STL%20Subset-0EA5E9?style=for-the-badge" />
  <img src="https://img.shields.io/badge/License-MIT-yellow?style=for-the-badge" />
  <img src="https://img.shields.io/badge/Header%20Only-Yes-22c55e?style=for-the-badge" />
</p>

# mySTL

一份课程向但可复用的 STL 核心容器实现：在纯头文件结构下落地 `vector`、`list`、`map`、`priority_queue` 的关键语义路径，重点突出**可读性、复杂度意识、工程边界定义**。

> 不追求标准库级性能极限，而是把“底层实现能解释清楚”作为第一优先级，适合面试、课程答辩、算法复盘、开源展示。

---

- [项目价值](#项目价值)
- [已实现能力](#已实现能力)
- [架构与文件组织](#架构与文件组织)
- [复杂度与设计要点](#复杂度与设计要点)
- [接入示例](#接入示例)
- [清理与收敛](#清理与收敛)
- [可拓展方向](#可拓展方向)
- [许可证](#许可证)

## 项目价值

- **学习价值**：通过手写实现掌握内存扩容、迭代器失效、树结构重平衡、异常安全等关键场景。
- **展示价值**：能快速证明你对 C++ 容器模型和复杂度模型的理解，不止会“调 API”。
- **复用价值**：头文件可直接用于课程作业、编程题库对比和自定义容器实验环境。

## 已实现能力

| 容器 | 关键能力 | 学习价值 |
|---|---|---|
| `vector<T>` | 动态扩容、随机访问、迭代器前进后退、边界控制 | 内存布局、容量策略、摊还复杂度 |
| `list<T>` | 双向链表节点管理、双端增删、双向遍历 | 指针维护、边界条件、稳定性 |
| `map<K,V>` | 红黑树基础行为、键值映射、有序遍历 | 平衡树维护与有序集合抽象 |
| `priority_queue<T>` | 入队/出队、堆序列维护、比较策略 | 堆模型与任务调度思路 |

## 架构与文件组织

```text
mySTL/
├─ vector.hpp            # 线性容器
├─ list.hpp              # 链表容器
├─ map.hpp               # 红黑树映射
├─ priority_queue.hpp    # 堆容器
└─ utility/
   ├─ algorithm.hpp      # 低层辅助算法
   ├─ exceptions.hpp     # 自定义异常语义
   └─ utility.hpp        # 通用帮助函数
```

## 复杂度与设计要点

- `vector`
  - 访问：O(1)
  - 头尾插入/删除（尾端）：均摊 O(1)
  - 说明：以连续内存为主，换取遍历与随机访问性能；代价是容量重分配与迭代器失效。

- `list`
  - 插入/删除：O(1)
  - 遍历：O(n)
  - 说明：节点级别高内聚，适合需要局部变更的场景，但随机访问不适用。

- `map`
  - 查找/插入/删除：O(log n)
  - 说明：红黑树保持近似平衡，换取有序访问与范围查询特性。

- `priority_queue`
  - 插入/取顶：O(log n)
  - 查看顶部：O(1)
  - 说明：适合任务调度、贪心场景的基础数据结构。

## 接入示例

```cpp
#include "vector.hpp"
#include "map.hpp"

int main() {
  my_stl::vector<int> nums{1, 2, 3, 4};
  my_stl::map<int, int> scores;
  scores[1] = 100;

  nums.push_back(5);
  nums.pop_back();
  if (scores.find(1) != scores.end()) {
    return nums.empty() ? 1 : 0;
  }
  return 1;
}
```

```bash
g++ -std=c++17 -I. -O2 -o example example.cpp
./example
```

> 说明：仓库聚焦源码演示，不绑定某一套测试框架；后续可直接添加 Catch2 / GoogleTest。

## 清理与收敛

- 已移除：课程期噪音文件、实验性临时产物（非实现核心）。
- 保留：核心容器源码、复用工具与 MIT 许可文件，确保对外展示链路清晰稳定。
- 结果：阅读路径更短，展示中更容易突出“能力”而非“杂物”。

## 可拓展方向

- [ ] 增加单元测试覆盖（边界值、异常分支、复杂度对比）
- [ ] 补齐迭代器契约（`const_iterator`、reverse、distance 等）
- [ ] 补齐更多 STL 兼容语义（allocator/异常策略分层）

## 许可证

MIT —— 详见 `LICENSE`。
