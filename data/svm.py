import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
from sklearn.svm import SVC
from sklearn.model_selection import train_test_split, GridSearchCV,cross_val_score
from sklearn.preprocessing import StandardScaler
from sklearn.metrics import accuracy_score
from tqdm import tqdm

# 读取特征和标签数据集（假设空格和回车分隔的txt文件）
X = pd.read_csv('result0.dat', delim_whitespace=True, header=None).values
y = pd.read_csv('classify.dat', delim_whitespace=True, header=None).values.ravel()

# 检查特征和标签数据的形状
print(f"Feature data shape: {X.shape}")
print(f"Label data shape: {y.shape}")

# 如果数据不一致，进行修剪
if X.shape[0] != y.shape[0]:
    min_samples = min(X.shape[0], y.shape[0])
    X = X[:min_samples]
    y = y[:min_samples]
    print(f"数据行数不一致，已修剪为相同样本数: {min_samples}")

# 将数据集分为训练集和测试集
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.3, random_state=42)

# 归一化数据
scaler = StandardScaler()
X_train_scaled = scaler.fit_transform(X_train)
X_test_scaled = scaler.transform(X_test)


# 定义多项式核 SVM 的参数范围
param_grid = { 'kernel': ['linear', 'rbf'],'C': [0.1, 1, 10, 100], 'gamma': ['scale', 'auto'],}  # 多项式核的参数

# 使用 GridSearchCV 和进度条，启用多核计算
grid_search = GridSearchCV(SVC(),param_grid, cv=10, n_jobs=-1, verbose=0)

print("正在进行超参数搜索（GridSearchCV）...")

# 使用 tqdm 包裹 fit 函数以添加进度条
with tqdm(total=1, desc="GridSearchCV", unit="step") as pbar:
    grid_search.fit(X_train_scaled, y_train)
    pbar.update(1)

# 输出最佳参数和评分
print(f"Best Parameters: {grid_search.best_params_}")
print(f"Best Cross-Validation Score: {grid_search.best_score_:.2f}")

# 获取最佳模型
best_model = grid_search.best_estimator_

# 在测试集上进行预测
y_pred = best_model.predict(X_test_scaled)

# 输出测试集的准确率
accuracy = accuracy_score(y_test, y_pred)
print(f"Test Accuracy: {accuracy:.2f}")

# 绘制数据和决策边界
def plot_decision_boundary(X, y, model):
    h = .02  # 网格步长
    x_min, x_max = X[:, 0].min() - 1, X[:, 0].max() + 1
    y_min, y_max = X[:, 1].min() - 1, X[:, 1].max() + 1
    xx, yy = np.meshgrid(np.arange(x_min, x_max, h), np.arange(y_min, y_max, h))

    # 预测网格点
    Z = []
    print("正在绘制决策边界...")
    for point in tqdm(np.c_[xx.ravel(), yy.ravel()], total=len(xx.ravel())):
        Z.append(model.predict([point])[0])
    
    Z = np.array(Z).reshape(xx.shape)

    # 绘制决策边界
    plt.contourf(xx, yy, Z, alpha=0.8)
    plt.scatter(X[:, 0], X[:, 1], c=y, edgecolors='k', marker='o')
    plt.title("SVM Decision Boundary with Polynomial Kernel")
    plt.show()

# 绘制测试集的决策边界
plot_decision_boundary(X_train_scaled, y_train, best_model)
