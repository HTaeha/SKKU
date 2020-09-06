import numpy as np

def Accuracy(label, pred):
    ########################################################################################
    # TODO : Complete the code to calculate the accuracy for prediction.
    #         [Input]
    #         - label : (N, ), Correct label with 0 (negative) or 1 (positive)
    #         - hypo  : (N, ), Predicted score between 0 and 1
    #         [output]
    #         - Acc : (scalar, float), Computed accuracy score
    # ========================= EDIT HERE =========================
    Acc = None

    true = 0
    for i in range(len(pred)):
        if pred[i] > 0.5:
            if label[i] == 1:
                true += 1
        else:
            if label[i] == 0:
                true += 1
    Acc = true/len(pred)


    # =============================================================
    return Acc

def Precision(label, pred):
    ########################################################################################
    # TODO : Complete the code to calculate the Precision for prediction.
    #         you should consider that label = 1 is positive. 0 is negative
    #         Notice that, if you encounter the divide zero, return 1
    #         [Input]
    #         - label : (N, ), Correct label with 0 (negative) or 1 (positive)
    #         - hypo  : (N, ), Predicted score between 0 and 1
    #         [output]
    #         - precision : (scalar, float), Computed precision score
    # ========================= EDIT HERE =========================
    precision = None

    TP = 0
    FP = 0
    for i in range(len(pred)):
        if pred[i] > 0.5:
            if label[i] == 1:
                TP += 1
            else:
                FP += 1

    if TP + FP == 0:
        precision = 1
    else:
        precision = TP/(TP+FP)


    # =============================================================
    return precision

def Recall(label, pred):
    ########################################################################################
    # TODO : Complete the code to calculate the Recall for prediction.
    #         you should consider that label = 1 is positive. 0 is negative
    #         Notice that, if you encounter the divide zero, return 1
    #         [Input]
    #         - label : (N, ), Correct label with 0 (negative) or 1 (positive)
    #         - hypo  : (N, ), Predicted score between 0 and 1
    #         [output]
    #         - recall : (scalar, float), Computed recall score
    # ========================= EDIT HERE =========================
    recall = None

    TP = 0
    FN = 0
    for i in range(len(pred)):
        if label[i] == 1:
            if pred[i] > 0.5:
                TP += 1
            else:
                FN += 1

    if TP + FN == 0:
        recall = 1
    else:
        recall = TP/(TP+FN)

    # =============================================================
    return recall

def F_measure(label, pred):
    ########################################################################################
    # TODO : Complete the code to calculate the F-measure score for prediction.
    #         you can erase the code. (F_score = 0.)
    #         Notice that, if you encounter the divide zero, return 1
    #         [Input]
    #         - label : (N, ), Correct label with 0 (negative) or 1 (positive)
    #         - hypo  : (N, ), Predicted score between 0 and 1
    #         [output]
    #         - F_score : (scalar, float), Computed F-score score
    # ========================= EDIT HERE =========================
    F_score = None

    recall = Recall(label, pred)
    precision = Precision(label, pred)

    if recall + precision == 0:
        F_score = 1
    else:
        F_score = (2*recall*precision)/(recall+precision)

    # =============================================================
    return F_score

def MAP(label, hypo, at = 10):
    ########################################################################################
    # TODO : Complete the code to calculate the MAP for prediction.
    #         Notice that, hypo is the real value array in (0, 1)
    #         MAP (at = 10) means MAP @10
    #         [Input]
    #         - label : (N, K), Correct label with 0 (incorrect) or 1 (correct)
    #         - hypo  : (N, K), Predicted score between 0 and 1
    #         - at: (int), # of element to consider from the first. (TOP-@)
    #         [output]
    #         - Map : (scalar, float), Computed MAP score
    # ========================= EDIT HERE =========================
    Map = None

    AP = []
    for i in range(len(label)):
        true_label = 0
        ap = 0
        ind = np.argsort(hypo[i])
        ind = ind[::-1]
        for j, data in enumerate(ind):
            if label[i][data] == 1:
                true_label += 1
                if j < at:
                    ap += true_label/(j+1)
        AP.append(ap/true_label)
    Map = np.mean(AP)
    # =============================================================
    return Map

def nDCG(label, hypo, at = 10):
    ########################################################################################
    # TODO : Complete the each code to calculate the nDCG for prediction.
    #         you can erase the code. (dcg, idcg, ndcg = 0.)
    #         Notice that, hypo is the real value array in (0, 1)
    #         nDCG (at = 10 ) means nDCG @10
    #         [Input]
    #         - label : (N, K), Correct label with 0 (incorrect) or 1 (correct)
    #         - hypo  : (N, K), Predicted score between 0 and 1
    #         - at: (int), # of element to consider from the first. (TOP-@)
    #         [output]
    #         - Map : (scalar, float), Computed nDCG score


    def DCG(label, hypo, at=10):
        # ========================= EDIT HERE =========================
        dcg = None

        dcg = 0
        ind = np.argsort(hypo)
        ind = ind[::-1]
        for i, data in enumerate(ind):
            if label[data] == 1:
                if i < at:
                    dcg += 1/(np.log2(i+2))
                else:
                    break
        # =============================================================
        return dcg

    def IDCG(label, hypo, at=10):
        # ========================= EDIT HERE =========================
        idcg = None
        
        idcg = 0
        true_label = 0
        for i in range(len(label)):
            if label[i] == 1:
                true_label += 1
        min_val = min(true_label, at)
        for i in range(min_val):
            idcg += 1/(np.log2(i+2))

        # =============================================================
        return idcg
    # ========================= EDIT HERE =========================
    ndcg = None
    ndcg = 0
    for i in range(len(label)):
        ndcg += DCG(label[i], hypo[i], at)/IDCG(label[i], hypo[i], at)
    ndcg /= len(label)
    # =============================================================
    return ndcg

# =============================================================== #
# ===================== DO NOT EDIT BELOW ======================= #
# =============================================================== #

def evaluation_test1(label, pred, at = 10):
    result = {}

    result['Accuracy '] = Accuracy(label, pred)
    result['Precision'] = Precision(label, pred)
    result['Recall   '] = Recall(label, pred)
    result['F_measure'] = F_measure(label, pred)

    return result

def evaluation_test2(label, hypo, at = 10):
    result = {}

    result['MAP  @%d'%at] = MAP(label, hypo, at)
    result['nDCG @%d'%at] = nDCG(label, hypo, at)

    return result
