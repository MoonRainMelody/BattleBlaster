# 坦克射击闯关小游戏（BattleBlaster）

> Unreal Engine 5.6 · C++ · 18 个源文件 / 1023 行 · **8 个玩法类** · 3 关完整可玩
> 鼠标瞄准炮塔、WASD 驾驶坦克，逐关摧毁全部敌方炮塔即可通关，被击毁则重开当前关。
> 命名对应：作品集中的展示名为「坦克射击闯关小游戏」，代码 / 仓库 / 类前缀代号为 `BattleBlaster`。

个人项目，**核心玩法逻辑全部由 C++ 实现**，蓝图只承担资产装配与外观。

---

## 技术要点

| 方向 | 实现 |
|---|---|
| 关卡循环 | 进度由 `UGameInstance` 承载（`CurrentLevelIndex` / `LastLevelIndex` + `Level_%d` 命名约定）；开局统计炮塔数量，坦克阵亡判负、炮塔清零判胜，延时后切关 / 重开 |
| 架构 | 继承 + 组合混用：`ABasePawn` 抽取坦克（`ATank`）与炮塔（`AEnemyTower`）的共有行为（炮塔旋转、发射、死亡表现）；`UHealthComponent` 以组件方式组合，可挂到任意可受伤 Actor |
| 攻击与表现分离 | `Fire()` 只在 Socket 生成弹丸并设置 Owner，命中判定与伤害放在 `AProjectile::OnHit`，把「谁受伤」与「谁发射」解耦；弹丸由 `UProjectileMovementComponent` 驱动 |
| 性能取舍 | 射程判定与开火由定时器驱动，只有需要连续平滑的炮塔旋转留在 `Tick`，避免无谓的每帧开销 |
| 生命周期治理 | 跨帧持有的对象引用统一补上 `UPROPERTY` 交由 GC 追踪（跨帧 Actor / 控件引用改用 `TObjectPtr`）；定时器从 `BeginPlay` 局部变量改为成员句柄，并在销毁路径 `ClearTimer` |
| 数据驱动 | 速度、转向率、射速、射程、伤害、血量全部 `EditAnywhere` 暴露，调参无需重编译 |
| 表现层 | Niagara 死亡 / 命中特效、三类音效、相机抖动；开场 `Get Ready! → 3 → 2 → 1 → Go!!` 倒计时期间锁定输入 |

C++ 玩法类清单：`ABasePawn` / `ATank` / `AEnemyTower` / `AProjectile` / `ABattleBlasterGameMode` / `UBattleBlasterGameInstance` / `UHealthComponent` / `UScreenMessage`。

---

## 目录结构

```
Source/
├── BattleBlaster/         8 个玩法类（见上表）
├── BattleBlaster.Target.cs
└── BattleBlasterEditor.Target.cs
Docs/
└── DesignDocument.md      设计文档（架构 / 系统拆解）
Config/                    DefaultEngine.ini / DefaultGame.ini 等
BattleBlaster.uproject     引擎关联 5.6，单 Runtime 模块
```

依赖模块：`Core` / `CoreUObject` / `Engine` / `InputCore` / `EnhancedInput` / `UMG` / `Niagara`。

---

## 构建

1. 安装 **Unreal Engine 5.6**；
2. 右键 `BattleBlaster.uproject` → *Generate Visual Studio project files*；
3. 打开生成的 `BattleBlaster.sln`，构建 `Development Editor` 配置，或直接用引擎打开 `BattleBlaster.uproject`。

---

## 仓库范围

本仓库包含**完整工程内容**：`Source/`、`Config/`、`Docs/`、`*.uproject` 以及 `Content/`（蓝图、关卡、材质、网格、特效，约 4 MB）。

- 克隆后用 UE 5.6 打开 `BattleBlaster.uproject` 即可直接运行（首次打开会提示编译 C++ 模块）。
- `.gitignore` 已排除 `Binaries/`、`Intermediate/`、`Saved/`、`DerivedDataCache/`、`.vs/` 等生成物与 IDE 缓存。

演示视频与截图见作品集：<https://dcnzeir0z0se.feishu.cn/docx/AanxdVrWUoTlVzxRowAcQBi3nyh>
