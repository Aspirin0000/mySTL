# mySTL

<p align="center">
  <img src="https://img.shields.io/badge/Language-C%2B%2B17-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white" />
  <img src="https://img.shields.io/badge/Focus-Data%20Structures-0EA5E9?style=for-the-badge" />
  <img src="https://img.shields.io/badge/License-MIT-yellow?style=for-the-badge" />
  <img src="https://img.shields.io/badge/Type-Header--Only-22c55e?style=for-the-badge" />
</p>

> 轻量 STL 重写工程：用现代 C++17 从头实现核心容器。

## 🧭 项目速览

- 目标：通过手写实现理解容器的复杂度、内存布局与迭代器语义。
- 产出：`vector`、`list`、`priority_queue`、`map`（红黑树）。
- 风格：课程导向、源码可读、接口尽量贴近 STL。

## 📦 已实现容器

| 容器 | 特性 | 时间复杂度 | 学习价值 |
|---|---|---|---|
| `vector` | 动态扩容、随机访问、迭代器 | 均摊 O(1) / O(1) | 连续存储与容量管理 |
| `list` | 双向链表、节点插入删除 | O(1) | 指针结构与稳定迭代 |
| `priority_queue` | 堆结构 | O(log n) | 优先级抽取 |
| `map` | 红黑树（RB-Tree） | O(log n) | 平衡树与有序字典 |

## 🧱 文件结构

- `vector.hpp`, `list.hpp`, `priority_queue.hpp`, `map.hpp`
- `utility/`（节点、异常、安全工具）
- `LICENSE`

## 🧪 快速验证

```cpp
#include "vector.hpp"
#include "map.hpp"

int main() {
  my_stl::vector<int> v = {1,2,3};
  my_stl::map<int,int> m;
  m[1] = 10;
  return v.size() == 3 && m.size() == 1 ? 0 : 1;
}
```

```bash
g++ -std=c++17 -I. -O2 example.cpp -o example
./example
```

## 🧼 项目清理说明

- 仅保留核心源码与最小文档；移除了课程期临时产物和噪音文件。
- 保留 `LICENSE` 便于展示与复用。

## 🛣️ 扩展方向

- [ ] 增加 iterator debug 语义
- [ ] 补齐单元测试套件与 CI
- [ ] 增加更完整的异常安全场景说明

## 🛡️ License

MIT — see `LICENSE`.
