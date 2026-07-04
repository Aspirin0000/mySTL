# mySTL

<p align="center">
  <img src="https://img.shields.io/badge/Language-C%2B%2B17-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white" />
  <img src="https://img.shields.io/badge/Focus-Data%20Structures-0EA5E9?style=for-the-badge" />
  <img src="https://img.shields.io/badge/License-MIT-yellow?style=for-the-badge" />
</p>

> A clean-room reimplementation of core STL containers for systems learning.

## 🎯 Overview

- **Implemented containers**: `vector`, `list`, `priority_queue`, `map` (Red-Black Tree)
- **Language**: Header-only C++17
- **目标**: 通过手写实现理解容器内存布局、迭代器语义与复杂度边界

## 🗂️ Structure

- `vector.hpp`, `list.hpp`, `priority_queue.hpp`, `map.hpp`
- `utility/` (辅助节点/迭代器/算法组件)
- `LICENSE`

## 🔧 Build / Use

This project is header-only. Include the relevant headers directly:

```cpp
#include "vector.hpp"
#include "map.hpp"
```

### Quick check (optional)

```bash
# If you create your own example.cpp
g++ -std=c++17 -I. example.cpp -o example
./example
```

## ✨ Why this project exists

- 练手 STL 核心容器设计（模板、资源管理、异常安全）
- 作为课程/面试准备的复盘素材
- 展示 C++ 底层系统设计能力（不是直接调用 STL）

## 🛡️ License

MIT License — see `LICENSE`.
