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

│ ├── scene/ # 场景头文件

│ │ ├── scene.h # 场景基类声明

│ │ ├── game_scene.h # 游戏场景声明

│ │ ├── menu_scene.h # 主菜单场景声明

│ │ ├── selector_scene.h # 选人场景声明

│ ├── animation.h # 动画声明

│ ├── atlas.h # 图集类声明

│ ├── platform.h # 碰撞类声明

│ ├── camera.h # 摄像机声明

│ ├── Utill.h # 工具声明

│ ├── vector2.h # 坐标位置声明

│ ├── scene_manager.h # 场景管理器声明

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
   
3. 使用IDE打开项目并编译运行.



## 目前待完成功能

- [ ] 玩家类设计和角色移动基础
- [ ] 平台单向碰撞检测和重力模拟
- [ ] 角色技能设计和子弹基类实现
- [ ] 玩家子弹派生类详细实现
- [ ] 玩家子弹发射和角色技能实现
- [ ] 无敌帧和玩家状态栏实现
- [ ] 粒子系统和角色特效动画实现
- [ ] 胜负检定和结算动效实现

👨‍💻 开发者: nuomimuyouci

📫 联系信息: https://github.com/tuzibuqiahuluobo \ 2672172829@qq.com
