
import matplotlib.pyplot as plt

# 读取文档内容
def read_file(file_path):
    x, y, z,t,u = [], [], [],[],[]
    with open(file_path, 'r') as file:
        for line in file:
            # 假设数据之间用空格分隔
            data = line.split()
            # 将数据转换为浮点数并分别添加到x, y, z
            if len(data) == 4:
                x.append(float(data[0]))
                y.append(float(data[1]))
                z.append(float(data[2]))
                t.append(float(data[3]))
                u.append(float(30))
    return x, y, z,t,u

# 生成折线图
def plot_data(x, y,u,labelx,labely):
    plt.rcParams["font.family"]='SimHei'
    plt.plot(x, label=labelx, marker='o')
    plt.plot(y, label=labely, marker='x')
    plt.plot(u, label='真实值', linestyle=':')
    plt.xlabel('Index')
    plt.ylabel('Values')
    plt.title('Line Plot of '+labelx+","+labely)
    plt.legend()
    plt.show()

# 示例调用
file_path = './comp.dat'  # 替换为你的文件路径
x, y, z,t ,u= read_file(file_path)
plot_data(x, t,u,'自适应融合法',"测量值")
plot_data(x, z,u,'自适应融合法',"加权平均值")