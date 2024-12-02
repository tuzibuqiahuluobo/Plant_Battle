# Plant_Battle  

## 植物明星大乱斗  

**Plant_Battle** 是一款充满趣味的 2D 对战游戏，玩家可以选择植物角色，在平台间跳跃移动、攻击对手，享受激烈的对战体验！  

---

## 游戏玩法 🌱  

- **双人对战**：两位玩家分别操控不同的植物角色。  
- **平台竞技**：在平台之间跳跃、移动，寻找合适的战斗位置。  
- **多样攻击**：使用普通攻击和特殊攻击对对方造成伤害。  
- **击败对手**：通过减少对手的生命值或将其击落平台来取得胜利。  

---

## 项目结构 📁  (待完善)

Plant_Battle/

├── src/ # 游戏核心源代码

│ ├── main.cpp # 程序入口

│ ├── GameEngine.cpp # 游戏引擎相关代码(暂无)

│ ├── Player.cpp # 玩家逻辑实现（暂无）

│ ├── Attack.cpp # 攻击逻辑与技能实现（暂无）

│ └── Platform.cpp # 平台和物理相关代码（暂无）

│

├── include/ # 头文件目录

│ ├── animation.h # 动画头文件

│ ├── atlas.h # 图集类头文件

│ ├── camera.h # 摄像机头文件

│ ├── platform.h # 物理模拟头文件

│ ├── Utill.h # 工具头文件

│ ├── vector2.h # 坐标位置头文件

│ ├── scene_manager.h # 场景管理器头文件

│ ├── scene/ # 场景筛选器

│ │ ├── scene.h # 场景基类头文件

│ │ ├── game_scene.h # 游戏场景头文件

│ │ ├── menu_scene.h # 主菜单场景头文件

│ │ ├── selector_scene.h # 选人场景头文件

│ ├── player/ # 角色筛选器

│ │ ├── player.h # 角色基类头文件

│ │ ├── player_id.h # 角色id基类头文件

│ │ ├── peashooter_player.h # 豌豆射手基类头文件

│ │ ├── sunflower_player.h # 龙日葵基类头文件

│ ├── bullet/ # 子弹筛选器

│ │ ├── bullet.h # 子弹基类头文件

│ │ ├── pea_bullet.h # 豌豆子弹头文件

│ │ ├── sun_bullet.h # 阳光炸弹子弹头文件

│ │ ├── sun_bullet_ex.h # 超级阳光炸弹子弹头文件

│ └── timer.h # 计时器声明

│

├── resources/ # 资源文件

│

---

## 技术栈与工具 🛠️  

- **编程语言**：C++

- **图形库**：EasyX  

---

## 如何运行 🕹️  

1. 确保本地已安装 **EasyX 图形库** 和兼容的 C++ 编译环境（如 Visual Studio 或 Code::Blocks）。

2. 克隆项目代码：  

   ```bash
   git clone https://github.com/your-repo/Plant_Battle.git
   cd Plant_Battle
   ```

3. 使用IDE打开项目并编译运行.

## 待完成任务计划

- [x] 场景管理器和图集类实现
- [x] 资源加载和动画类实现
- [x] 游戏摄像机基础实现
- [x] 通用定时器
- [x] 主菜单界面和角色选择界面搭建
- [x] 玩家选择界面动态效果及交互
- [x] 游戏局内场景搭建和物理模拟基础
- [x] 玩家类设计和角色移动基础
- [x] 平台单向碰撞检测和重力模拟
- [x] 角色技能设计和子弹基类实现
- [ ] 玩家子弹派生类实现
- [ ] 玩家子弹发射和角色技能实现
- [ ] 无敌帧和玩家状态栏实现
- [ ] 粒子系统和角色特效动画实现
- [ ] 胜负检定与结算动效实现

### 该项目基于bilibili视频网站的UP主：Voidmatrix 的项目修改而来 

### 这是视频链接：https://www.bilibili.com/video/BV1jx4y1t7eP?spm_id_from=333.788.videopod.sections&vd_source=9ecf2e76349222089b258a9e0960e34e

贡献 🙌
欢迎任何形式的贡献！如果你有好的想法或发现了问题，欢迎提交 Issue 或 Pull Request。


开发者 👨‍💻
开发者: nuomimuyouci
联系信息: https://github.com/tuzibuqiahuluobo
