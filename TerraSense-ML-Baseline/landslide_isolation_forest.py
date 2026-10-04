"""
TerraSense - baseline landslide anomaly detection (Isolation Forest)
Dataset: Pukrongta et al. 2025, "Raw Sensor Data - Landslide Monitoring" (Mendeley Data, DOI 10.17632/9w43sg73bt.1)
Purpose: demonstrate the ML pipeline only. NOT trained on TerraSense data. No event labels exist, so this is unsupervised.
"""
import pandas as pd, numpy as np
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
from sklearn.ensemble import IsolationForest

FILE = "Raw_Sensor_Data_Landslide_Monitoring__1_.xlsx"   # put next to this script
FEATURES = ["Rotation X", "Rotation Y", "Soil20cm", "Soil40cm", "Soil60cm",
            "Raindrop", "Vibration", "Temperature", "Humidity"]

def prepare(sheet):
    df = pd.read_excel(FILE, sheet_name=sheet).sort_values("Date").reset_index(drop=True)
    n0 = len(df)
    # remove start-up / placeholder rows (sensors reading 0 before settling)
    bad = (df["Temperature"] == 0) | (df["Humidity"] == 0) | \
          ((df["Soil20cm"] == 0) & (df["Soil40cm"] == 0) & (df["Soil60cm"] == 0))
    df = df[~bad].reset_index(drop=True)
    # simple rate-of-change feature for soil moisture
    for c in ["Soil20cm", "Soil40cm", "Soil60cm"]:
        df[c + "_rate"] = df[c].diff().fillna(0)
    return df, n0

results = []
fig, axes = plt.subplots(2, 1, figsize=(10, 6), sharex=False)
for ax, sheet in zip(axes, ["Node1", "Node2"]):
    df, n0 = prepare(sheet)
    feats = FEATURES + ["Soil20cm_rate", "Soil40cm_rate", "Soil60cm_rate"]
    split = int(len(df) * 0.7)                       # time-ordered split, no shuffling
    train, test = df.iloc[:split].copy(), df.iloc[split:].copy()
    model = IsolationForest(n_estimators=100, contamination=0.01, random_state=42)
    model.fit(train[feats])
    test["score"] = model.decision_function(test[feats])
    test["flag"] = model.predict(test[feats]) == -1
    event = (test["Vibration"] > 0) | (test["Raindrop"] > 0)   # sanity check only, NOT ground truth
    results.append((sheet, n0, len(df), len(train), len(test), int(test["flag"].sum()),
                    int(event.sum()), test.loc[event, "flag"].mean() if event.any() else float("nan"),
                    test.loc[~event, "flag"].mean()))
    ax.plot(test["Date"], test["score"], lw=0.6)
    ax.scatter(test.loc[test["flag"], "Date"], test.loc[test["flag"], "score"], s=6, c="red", label="flagged")
    ax.set_title(f"{sheet}: anomaly score on held-out period (lower = more unusual)")
    ax.legend(loc="lower left")
plt.tight_layout()
plt.savefig("landslide_anomaly_plot.png", dpi=150)

for r in results:
    print(f"{r[0]}: raw rows {r[1]}, after cleaning {r[2]}, train {r[3]}, test {r[4]}")
    print(f"   flagged in test: {r[5]} ({r[5]/r[4]:.1%})")
    print(f"   rows with rain/vibration activity in test: {r[6]}; flagged share {r[7]:.1%} vs {r[8]:.1%} for the rest")
