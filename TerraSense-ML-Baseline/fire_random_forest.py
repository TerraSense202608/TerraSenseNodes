"""
TerraSense - baseline fire-risk classifier (Random Forest)
Dataset: Smoke Detection Dataset (Kaggle: deepcontractor/smoke-detection-dataset; field data by S. Blattmann).
Purpose: demonstrate the ML pipeline only. NOT trained on TerraSense data; sensors differ from ours.
"""
import pandas as pd
from sklearn.model_selection import train_test_split
from sklearn.ensemble import RandomForestClassifier
from sklearn.metrics import classification_report, confusion_matrix

FILE = "smoke_detection_iot.csv"       # the REAL csv downloaded from Kaggle (see note in chat)
LABEL = "Fire Alarm"
# temperature, humidity, gas and particulate (smoke-type) readings; our node measures temperature, humidity, smoke, gas
FEATURES = ["Temperature[C]", "Humidity[%]", "TVOC[ppb]", "eCO2[ppm]", "Raw H2", "Raw Ethanol",
            "PM1.0", "PM2.5", "NC0.5", "NC1.0", "NC2.5"]   # pressure, counter (CNT) and time are left out on purpose

df = pd.read_csv(FILE)
df = df.dropna(subset=FEATURES + [LABEL])
print(len(df), "rows;", int(df[LABEL].sum()), "alarm /", int((df[LABEL] == 0).sum()), "no alarm")

# Chronological split: first 70% train, last 30% test (avoids leakage between neighbouring seconds)
if "UTC" in df.columns:
    df = df.sort_values("UTC")
split = int(len(df) * 0.7)
train, test = df.iloc[:split], df.iloc[split:]

model = RandomForestClassifier(n_estimators=100, random_state=42, class_weight="balanced")
model.fit(train[FEATURES], train[LABEL])
pred = model.predict(test[FEATURES])

print(classification_report(test[LABEL], pred, target_names=["no alarm", "fire alarm"]))
print("confusion matrix [[TN FP],[FN TP]]:\n", confusion_matrix(test[LABEL], pred))
print(dict(zip(FEATURES, model.feature_importances_.round(3))))
