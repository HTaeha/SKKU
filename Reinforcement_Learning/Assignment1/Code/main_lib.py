import numpy as np

'''
def policy_evaluation(env, policy, gamma=0.99, theta=1e-8):
    V = np.zeros(env.nS)

    return V

def policy_improvement(env, V, gamma=0.99):
    policy = np.zeros([env.nS, env.nA]) / env.nA

    return policy
'''

def policy_iteration(env, gamma=0.99, theta=1e-8):
    V = np.zeros(env.nS)
    policy = np.ones([env.nS, env.nA]) / env.nA
    
    while True:
        while True:
            variance = 0
            for i in range(env.nS):
                old = V[i]
                new_val = 0
                #update V
                for a in range(env.nA):
                    for s in range(len(env.MDP[i][a])):
                        new_val += env.MDP[i][a][s][0]*policy[i][a]*(env.MDP[i][a][s][2] + gamma*V[env.MDP[i][a][s][1]])
                V[i] = new_val
                variance = max(variance, abs(V[i] - old))
            
            if variance < theta:
                break

        policy_stable = True

        for i in range(env.nS):
            old_action = get_max_index(policy[i])
            temp = np.zeros(env.nA)
            for a in range(env.nA):
                q = 0
                for s in range(len(env.MDP[i][a])):
                    q += env.MDP[i][a][s][0]*(env.MDP[i][a][s][2] + gamma*V[env.MDP[i][a][s][1]])
                temp[a] = q
            max_q = max(temp)
            policy[i] = 0
            for index in range(len(temp)):
                if temp[index] == max_q:
                    policy[i][index] = 1
                    break
            new_action = get_max_index(policy[i]);
            if old_action != new_action:
                policy_stable = False

        if policy_stable:
            break

    return policy, V

def value_iteration(env, gamma=0.99, theta=1e-8):
    V = np.zeros(env.nS)
    policy = np.ones([env.nS, env.nA]) / env.nA

    while True:
        variance = 0
        for i in range(env.nS):
            old = V[i]
            #update V
            temp_v = np.zeros(env.nA)
            for a in range(env.nA):
                new_val = 0
                for s in range(len(env.MDP[i][a])):
                    new_val += env.MDP[i][a][s][0]*(env.MDP[i][a][s][2] + gamma*V[env.MDP[i][a][s][1]])
                temp_v[a] = new_val
            V[i] = max(temp_v)
            variance = max(variance, abs(V[i] - old))
        
        if variance < theta:
            break

    for i in range(env.nS):
        temp = np.zeros(env.nA)
        for a in range(env.nA):
            q = 0
            for s in range(len(env.MDP[i][a])):
                q += env.MDP[i][a][s][0]*(env.MDP[i][a][s][2] + gamma*V[env.MDP[i][a][s][1]])
            temp[a] = q
        max_q = max(temp)
        policy[i] = 0
        for index in range(len(temp)):
            if temp[index] == max_q:
                policy[i][index] = 1
                break

    return policy, V

def get_max_index(List):
    maximum = 0
    for i, value in enumerate(List):
        if value > maximum:
            maximum = value
            index = i
    return index
