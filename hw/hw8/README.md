# Games101 HW8

## 前言

- hw8主题： 质点弹簧系统
- 任务：
  - 配置环境
  - 连接绳子约束
  - 半隐式欧拉法
  - 显式欧拉法
  - 显式Verlet
  - 阻尼
- 源码在：[yy6768/Games101: 学习Games101的笔记和自己的作业解决 (github.com)](https://github.com/yy6768/Games101)



## 安装环境

- 之前都是在本地windows非常的痛苦
- 后来参考这篇文章[在 Win10 下配置 GAMES101 开发环境（WSL2） - 知乎 (zhihu.com)](https://zhuanlan.zhihu.com/p/371080057)（我使用的是远程服务器，亲测远程服务器也能用，只要是支持X11的远程终端都可以用）
- 下载MobaXTerm： [https://mobaxterm.mobatek.net](https://link.zhihu.com/?target=https%3A//mobaxterm.mobatek.net/)

- 之后通过SSH即可以访问



## 连接绳子约束

在文件`rope.cpp`中：

![绳子样式](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230607113908703.png)

- 每个节点都有质量：质点
- 连接质点的是弹簧
- **pinned_nodes**设置节点的索引-对应节点的固定属性（真为固定）
- 构造Mass对象——质点

![绳子](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230607115835763.png)

```c++
Rope::Rope(Vector2D start, Vector2D end, int num_nodes, float node_mass,
           float k, vector<int> pinned_nodes) {
  // TODO (Part 1): Create a rope starting at `start`, ending at `end`, and
  // containing `num_nodes` nodes.
  Vector2D delta = end - start;
  for (int i = 0; i < num_nodes; i++) {
    masses.emplace_back(new Mass(start + delta * i, node_mass, false));
    if (i != 0) {
      springs.emplace_back(new Spring(masses[i], masses[i - 1], k));
    }
  }

  // Comment-in this part when you implement the constructor
  for (auto &i : pinned_nodes) {
    masses[i]->pinned = true;
  }
}
```



## 显式/半隐式欧拉法

![胡克定律](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230607120203033.png)

```c++
F=ma
v(t+1)=v(t)+a(t)dt
x(t+1)=x(t)+v(t)dt /For explicit method
x(t+1)=x(t)+v(t+1)dt /For semi-implicit method
```

如果不添加阻尼

- 显式欧拉方法非常诡异

- 半显示就挺好诡异

添加阻尼后

半显示非常真实（就是帧率有点低）

```c++
void Rope::simulateEuler(float delta_t, Vector2D gravity) {
  for (auto &s : springs) {
    // TODO (Part 2): Use Hooke's law to calculate the force on a node
    Vector2D delta = s->m2->position - s->m1->position;
    Vector2D delta_force =
        s->k * delta / delta.norm() * (delta.norm() - s->rest_length);
    s->m1->forces += delta_force;
    s->m2->forces += -delta_force;
  }
  bool is_explicit = false; // 控制显示/隐式
  for (auto &m : masses) {
    if (!m->pinned) {
      // TODO (Part 2): Add the force due to gravity, then compute the new
      // velocity and position
      m->forces += gravity * m->mass;
	  // TODO (Part 2): Add global damping
      if (is_explicit) {
        float k_d = 7;  // damping
        m->forces += -k_d * m->velocity;
      } else {
        float k_d = 0.01;  // damping
        m->forces += -k_d * m->velocity;
      }
      Vector2D a = m->forces / m->mass;
      Vector2D v = m->velocity;
      if (is_explicit) {
        m->velocity += a * delta_t;
        m->position += v * delta_t;
      } else {
        m->velocity += a * delta_t;
        m->position += m->velocity * delta_t;
      }
      
    }

    // Reset all forces on each mass
    m->forces = Vector2D(0, 0);
  }
}
```



## 显式 Verlet

- 如果不是无穷劲度系数弹簧，按照以下更新

```
×(t+1)=x(t)+[x(t)-x(t-1)]+a(t)*dt*dt
```

- 如果式无穷劲度系数弹簧，那么只要简单的移动每个质点的位置使得弹簧的长度保持原长。修正向量应该和两个质点之间的位移成比例，方向为一个质点指向另一 质点。每个质点应该移动位移的一半。
  - 两点定住则不需要管
  - 两点有一点定住，则另一点保证长度不变（距离不变）
  - 两点都没定住，则没变收到一半的力可以保持原常

```c++
void Rope::simulateVerlet(float delta_t, Vector2D gravity) {
  for (auto &s : springs) {
    // TODO (Part 3): Simulate one timestep of the rope using explicit Verlet
    // （solving constraints)
    if (s->k != -1) {
      Vector2D delta = s->m2->position - s->m1->position;
      Vector2D delta_force =
          s->k * delta / delta.norm() * (delta.norm() - s->rest_length);
      s->m1->forces += delta_force;
      s->m2->forces += -delta_force;
    }
  }

  for (auto &m : masses) {
    if (!m->pinned) {
      Vector2D temp_position = m->position;
      // TODO (Part 3.1): Set the new position of the rope mass
      m->forces += gravity * m->mass;
      Vector2D a = m->forces / m->mass;
      Vector2D v = m->velocity;
      Vector2D x_0 = m->last_position;
      Vector2D x_1 = m->position;
      Vector2D x_2 = x_1 + (x_1 - x_0) + a * delta_t * delta_t;
      m->last_position = x_1;
      m->position = x_2;
      // TODO (Part 4): Add global Verlet damping

     
    }
    m->forces = Vector2D(0, 0);
  }

  for (auto &s : springs) {
    if (s->k == -1) {
      Vector2D delta = s->m2->position - s->m1->position;
      Vector2D df_1 =
          0.5 * delta / delta.norm() * (delta.norm() - s->rest_length);
      Vector2D df_2 = -df_1;
      if (s->m1->pinned && s->m2->pinned) continue;
      if (s->m1->pinned) {
        df_2 *= 2;
        df_1 = Vector2D(0, 0);
      }
      if (s->m2->pinned) {
        df_1 *= 2;
        df_2 = Vector2D(0, 0);
      }

      s->m1->position += df_1;
      s->m2->position += df_2;
    }
  }
}
```

效果还是挺好的

就是最后会抽搐

![抽搐状态](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230607152119850.png)



## 显式Verlet的阻尼

![image-20230607152204587](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230607152204587.png)

直接按照公式即可

```c++
Vector2D x_2 = x_1 + (1 - damping) * (x_1 - x_0) + a * delta_t * delta_t;
```

![结果](http://typora-yy.oss-cn-hangzhou.aliyuncs.com/img/image-20230607152801527.png)

好像真的挺简单的，纯按照实验指导

就这样结束啦！