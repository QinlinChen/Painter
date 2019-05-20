绘图指令包括
- 重置画布
- 保存画布
- 设置画笔颜色
- 绘制线段
- 绘制多边形
- 绘制椭圆
- 绘制曲线
- 对图元平移
- 对图元旋转
- 对图元缩放
- 对线段裁剪

下面是对每条指令格式的说明。

### 重置画布
```
resetCanvas width height
```
- 指令描述
  - 清空当前画布，并重新设置宽高
- 参数说明
  - width, height: int 
    - 100 <= width, height <= 1000

### 保存画布
```
saveCanvas name
```
- 指令描述
  - 将当前画布保存为位图`name.bmp`
- 参数说明
  - name: string

### 设置画笔颜色
```
setColor R G B
```
- 参数说明
  - R, G, B: int

### 绘制线段
```
drawLine id x1 y1 x2 y2 algorithm
```
- 参数说明
  - id: int
    - 图元编号，每个图元的编号是唯一的
  - x1, y1, x2, y2 : float
    - 起点、终点坐标
  - algorithm: string
    - 绘制使用的算法，包括“DDA”和“Bresenham”

### 绘制多边形
```
drawPolygon id n algorithm
x1 y1 x2 y2 … xn yn
```
- 参数说明
  - id: int
    - 图元编号，每个图元的编号是唯一的
  - n: int
    - 顶点数
  - x1, y1, x2, y2, ..., xn, yn: float
    - 顶点坐标
  - algorithm: string
    - 绘制使用的算法，包括“DDA”和“Bresenham”

### 绘制椭圆
```
drawEllipse id x y rx ry
```
- 参数说明
  - id: int
    - 图元编号，每个图元的编号是唯一的
  - x, y: float
    - 圆心坐标
  - rx, ry: float
    - 长短轴半径

### 绘制曲线
```
drawCurve id n algorithm
x1 y1 x2 y2 … xn yn
```
- 参数说明
  - id: int
    - 图元编号，每个图元的编号是唯一的
  - n: int
    - 控制点数量
  - x1, y1, x2, y2, ..., xn, yn: float
    - 控制点坐标
  - algorithm: string
    - 绘制使用的算法，包括“Bezier”和“B-spline”

### 对图元平移
```
translate id dx dy
```
- 参数说明
  - id: int
    - 要平移的图元编号
  - dx, dy: float
    - 平移向量

### 对图元旋转
```
rotate id x y r
```
- 参数说明
  - id: int
    - 要旋转的图元编号
  - x, y: float
    - 旋转中心
  - r: float
    - 顺时针旋转角度（°）

### 对图元缩放
```
scale id x y s
```
- 参数说明
  - id: int
    - 要缩放的图元编号
  - x, y: float
    - 缩放中心
  - s: float
    - 缩放倍数

### 对线段裁剪
```
clip id x1 y1 x2 y2 algorithm
```
- 参数说明
  - id: int
    - 要裁剪的图元编号
  - x1, y1, x2, y2: float
    - 裁剪窗口左下、右上角坐标
  - algorithm: string
    - 裁剪使用的算法，包括“Cohen-Sutherland”和“Liang-Barsky”
