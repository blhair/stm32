# 📘 Git 使用与 CubeMX 项目管理实践指南

本指南适用于使用 STM32CubeMX 创建工程，并希望用 Git 进行版本控制与多人协作的开发者。内容涵盖基础 Git 命令、高阶使用技巧、冲突处理以及 .ioc 文件的特殊管理建议。

---

## 目录

1. [教程推荐](#教程推荐)
2. [常用 Git 工具对比](#常用-git-工具对比)
3. [Git 基础操作](#git-基础操作)
    - [仓库初始化](#仓库初始化)
    - [文件操作](#文件操作)
    - [分支管理](#分支管理)
    - [远程仓库操作](#远程仓库操作)
4. [CubeMX 项目与 .ioc 文件管理](#cubemx-项目与-ioc-文件管理)
5. [示例 .gitignore](#示例-gitignore)
6. [高效使用建议](#高效使用建议)
7. [常用命令速查表](#常用命令速查表)

---

## 📚 教程推荐

### 1. [Missing Semester Git 章节（MIT）](https://missing.csail.mit.edu/2020/version-control/)
- 使用 bash 命令行进行讲解，内容覆盖全面；
- 学完后能胜任 80% 以上实际场景；
- 配套练习题加深理解；
- 课程 note 中有大量延伸资料；
- **缺点：** 对新手略有门槛，内容偏硬核。

### 2. [Learn Git Branching](https://learngitbranching.js.org/?locale=zh_CN)
- 可视化的 Git 闯关类教学网站；
- 零基础也可以快速掌握 Git 基本命令与操作逻辑；
- 可以帮助优化命令顺序与思维方式；
- **缺点：** 与真实项目实践略有脱节，有些命令简化处理。

### 3. [Pro Git（官方 Git 教程）](https://git-scm.com/book/zh/v2)
- 一本全面、系统的 Git 教程，适合中高级用户；
- 对 Git 的工作原理、对象模型讲解深入；
- **建议：** 不必通读，可作为工具书查阅使用。

### 4. 其他资源
- B 站上有大量 Git 入门/进阶视频教程，适合喜欢视频学习的用户；
- 推荐搜索关键词：“Git 入门”、“Git 分支”、“Git 冲突解决”等。

---

## 🛠️ 常用 Git 工具对比

| 工具                  | 优点                   | 使用建议                       |
|-----------------------|------------------------|--------------------------------|
| 命令行 bash           | 原始方式，自由度高     | 建议掌握，适合重度用户         |
| VSCode Git            | 可视化界面，集成终端   | 推荐所有人，特别是初学者       |
| GitHub / GitLab       | 云仓库，便于协作       | 远程同步、多人开发首选         |
| STM32CubeIDE / VS2022 | 集成化 Git 支持        | 适合 IDE 党，但功能相对简单    |

---

## 🚀 Git 基础操作

### 仓库初始化

```bash
git init
```

### 文件操作

**添加文件并提交：**
```bash
git add .
git commit -m "首次提交：添加CubeMX配置与生成代码"
```

### 分支管理

**创建并切换新分支：**
```bash
git checkout -b dev
```

**切换分支：**
```bash
git checkout main
```

**合并分支：**
```bash
git merge dev
```

**将当前分支 rebase 到 main：**
```bash
git rebase main
```
> 用途：将当前分支的提交，移动到 main 分支最后一次提交之后。

**示例：**
```bash
git checkout feature
git rebase main
```

**常用后缀：**
- 交互式修改历史提交：
  ```bash
  git rebase -i HEAD~3
  ```
- 取消 rebase 操作：
  ```bash
  git rebase --abort
  ```

### 远程仓库操作

**添加远程仓库地址：**
```bash
git remote add origin https://github.com/user/repo.git
```

**推送本地提交：**
```bash
git push origin main
```

**拉取远程更新：**
```bash
git pull
```

---

## 🧩 CubeMX 项目与 .ioc 文件管理

`.ioc` 是 STM32CubeMX 的核心配置文件，结构类似 INI 格式。  
Git 合并时如果遇到冲突，通常如下所示：

```text
PA0.Signal=GPIO_Output
<<<<<<< HEAD
PA1.Signal=ADC1_IN1
=======
PA1.Signal=GPIO_Input
>>>>>>> other-branch
PB0.Signal=TIM3_CH3
```

**合并 .ioc 文件建议：**
- 不要保留 `<<<<<<<`、`=======`、`>>>>>>>` 等标记符；
- 手动选择正确的配置项，删除冲突标记；
- CubeMX 无法打开损坏的 .ioc 文件，请合并后测试能否正常打开；
- 建议由一个团队成员专门负责 .ioc 文件修改，其他人只使用不改动；
- .ioc 文件修改完立即提交，防止堆积后产生冲突。

---

## 📦 示例 .gitignore

建议放在项目根目录：

```gitignore
# 构建产物
*.o
*.d
*.elf
*.hex
*.map
*.list

# IDE文件
/Debug/
/Release/
/build/
/STM32CubeIDE/

# CubeMX 用户配置文件（非必须）
*.mxproject
*.launch
```

---

## 🧠 高效使用建议

| 情况                   | 建议                                         |
|------------------------|----------------------------------------------|
| 团队多人开发           | 每人独立分支 + 合并前提交 PR                 |
| 修改配置文件（如 .ioc） | 仅限一人操作 + 修改后立即提交                 |
| 提交规范               | 使用语义化提交说明（如 feat, fix, config）    |
| 查看提交历史           | `git log --graph --oneline --all` 一图看懂分支关系 |

---

## 🔖 常用命令速查表

| 操作                   | 命令                                               |
|------------------------|----------------------------------------------------|
| 查看状态               | `git status`                                       |
| 查看修改               | `git diff`                                         |
| 查看日志               | `git log`                                          |
| 创建标签               | `git tag v1.0`                                     |
| 暂存修改               | `git stash`                                        |
| 回滚到上次提交         | `git reset --hard HEAD`                            |
| 拉取并重设为远程版本   | `git fetch origin && git reset --hard origin/main` |
| 树状化简略展示git日志  | `git log --graph --oneline --all`                   |