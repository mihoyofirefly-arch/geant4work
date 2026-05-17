# import pyg4ometry

# step_file = "/home/chao/geant4/workdir/B1try/models/all_structure.STEP"
# gdml_file = "/home/chao/geant4/workdir/B1try/models/all_structure.gdml"

# reader = pyg4ometry.pyoce.Reader(step_file)
# fs = reader.freeShapes()

# worldName = pyg4ometry.pyoce.pythonHelpers.get_TDataStd_Name_From_Label(fs.Value(1))

# # 给命名对象分配材料；键是 CAD 里的 body 名字
# mats = {
#     # "Shell": "G4_Al",
#     # "Crystal": "G4_Si",
#     # "Window": "G4_MYLAR",
# }

# # 控制某些 body 的网格细度；先留空，跑通后再细调
# mesh = {
#     # "Shell": {"linDef": 1.0, "angDef": 0.5},
# }

# # 不想转换的 body 名字
# skip = [
#     # "SmallScrew_01",
# ]

# reg = pyg4ometry.convert.oce2Geant4(
#     reader.shapeTool,
#     worldName,
#     mats,
#     skip,
#     mesh,
#     oceName=True
# )

# writer = pyg4ometry.gdml.Writer()
# writer.addDetector(reg)
# writer.write(gdml_file)

# print("Wrote:", gdml_file)

import sys
from pathlib import Path
import pyg4ometry

step_file = Path("/home/chao/geant4/workdir/lab101structure/models/all_structure.STEP")
gdml_file = Path("/home/chao/geant4/workdir/lab101structure/models/all_structure.gdml")


def seq_len(seq):
    # 常见接口 1：Length()
    if hasattr(seq, "Length"):
        try:
            return seq.Length()
        except Exception:
            pass

    # 常见接口 2：Size()
    if hasattr(seq, "Size"):
        try:
            return seq.Size()
        except Exception:
            pass

    # 常见接口 3：Extent()
    if hasattr(seq, "Extent"):
        try:
            return seq.Extent()
        except Exception:
            pass

    # Python 序列协议
    try:
        return len(seq)
    except Exception:
        pass

    # 兜底：尝试 1-based Value(i)
    n = 0
    while True:
        try:
            seq.Value(n + 1)
            n += 1
        except Exception:
            break
    return n


def seq_item(seq, i):
    # 这里 i 用 0-based
    # 优先尝试 OpenCascade 常见的 1-based Value()
    if hasattr(seq, "Value"):
        try:
            return seq.Value(i + 1)
        except Exception:
            pass

    # 再尝试 Python 下标
    try:
        return seq[i]
    except Exception:
        pass

    # 有些绑定可能也是 1-based __getitem__
    try:
        return seq[i + 1]
    except Exception:
        pass

    raise RuntimeError(f"无法从序列中读取第 {i} 个元素")


print("STEP file =", step_file)
print("Exists    =", step_file.exists())
if not step_file.exists():
    raise SystemExit("STEP 文件不存在，请检查路径。")

reader = pyg4ometry.pyoce.Reader(str(step_file))
fs = reader.freeShapes()

nfs = seq_len(fs)
print("Number of free shapes =", nfs)

if nfs < 1:
    raise SystemExit(
        "freeShapes() 没有可用元素：STEP 可能没有被成功解析成顶层 shape。"
    )

names = []
for i in range(nfs):
    lab = seq_item(fs, i)
    try:
        name_i = pyg4ometry.pyoce.pythonHelpers.get_TDataStd_Name_From_Label(lab)
    except Exception:
        name_i = f"<unnamed_{i+1}>"
    names.append(name_i)
    print(f"free shape {i+1}: {name_i}")

# 默认取第一个顶层 shape 名字
worldName = names[0]
print("Using worldName =", worldName)

mats = {}
mesh = {}
skip = []

reg = pyg4ometry.convert.oce2Geant4(
    reader.shapeTool,
    worldName,
    mats,
    skip,
    mesh,
    oceName=True
)

writer = pyg4ometry.gdml.Writer()
writer.addDetector(reg)
writer.write(str(gdml_file))

print("Wrote:", gdml_file)