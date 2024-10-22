import matplotlib.pyplot as plt

# 读取文档内容
def read_file(file_path):
    x, y, z ,t= [], [], [],[]
    with open(file_path, 'r') as file:
        for line in file:
            # 数据之间用空格分隔
            data = line.split()
            if len(data) == 4:
                x.append(float(data[0]))
                y.append(float(data[1]))
                z.append(float(data[2]))
                t.append(float(data[3]))
    return x, y, z,t

# 生成三个独立的折线图
def plot_data_separately(x, y, z,t):
    fig, axs = plt.subplots(4, 1, figsize=(8, 12))  # 创建3个子图

    # X数据折线图
    axs[0].plot(x, label='X values', marker='o')
    axs[0].set_title('X values')
    axs[0].set_xlabel('Index')
    axs[0].set_ylabel('X')
    axs[0].legend()

    # Y数据折线图
    axs[1].plot(y, label='Y values', marker='x', color='orange')
    axs[1].set_title('Y values')
    axs[1].set_xlabel('Index')
    axs[1].set_ylabel('Y')
    axs[1].legend()

    # Z数据折线图
    axs[2].plot(z, label='Z values', marker='s', color='green')
    axs[2].set_title('Z values')
    axs[2].set_xlabel('Index')
    axs[2].set_ylabel('Z')
    axs[2].legend()

    axs[3].plot(z, label='t values', marker='s', color='blue')
    axs[3].set_title('t values')
    axs[3].set_xlabel('Index')
    axs[3].set_ylabel('t')
    axs[3].legend()

    plt.tight_layout()  # 自动调整子图间距
    plt.show()

# 示例调用
file_path = './result.dat'  # 替换为你的文件路径
x, y, z ,t= read_file(file_path)
plot_data_separately(x, y, z,t)
