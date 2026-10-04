import pandas as pd
from sklearn.model_selection import train_test_split
from sklearn.ensemble import RandomForestClassifier
from sklearn.metrics import accuracy_score

df = pd.read_csv("smoke_detection_iot.csv").dropna()
LABEL = "Fire Alarm"
FEATURES = ["Temperature[C]", "Humidity[%]", "TVOC[ppb]", "eCO2[ppm]", "Raw H2", "Raw Ethanol",
            "PM1.0", "PM2.5", "NC0.5", "NC1.0", "NC2.5"]

df = df.sort_values("UTC")
cut = int(len(df) * 0.7)
tr_t, te_t = df.iloc[:cut], df.iloc[cut:]
m1 = RandomForestClassifier(n_estimators=100, random_state=42, class_weight="balanced")
m1.fit(tr_t[FEATURES], tr_t[LABEL])
print("time-ordered split accuracy:", round(accuracy_score(te_t[LABEL], m1.predict(te_t[FEATURES])), 3))

tr_r, te_r = train_test_split(df, test_size=0.3, random_state=42, stratify=df[LABEL])
m2 = RandomForestClassifier(n_estimators=100, random_state=42, class_weight="balanced")
m2.fit(tr_r[FEATURES], tr_r[LABEL])
print("random split accuracy:      ", round(accuracy_score(te_r[LABEL], m2.predict(te_r[FEATURES])), 3))
