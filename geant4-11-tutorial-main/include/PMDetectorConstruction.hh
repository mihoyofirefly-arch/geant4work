#ifndef PMDETECTORCONSTRUCTION_HH
#define PMDETECTORCONSTRUCTION_HH

#include "G4VUserDetectorConstruction.hh"//一个抽象基类，用户必须从这个类继承并实现Construct()方法来定义几何结构。

#include "G4Box.hh"//定义一个长方体几何体类。
#include "G4Sphere.hh"//定义一个球体几何体类。

#include "G4LogicalVolume.hh"//定义一个逻辑体类，逻辑体是几何体和材料的结合，它的作用是把一个几何形状 G4VSolid 和一种材料 G4Material 绑定起来。
#include "G4VPhysicalVolume.hh"//定义一个物理体类，物理体是逻辑体在空间中的实例化，它负责描述一个 G4LogicalVolume 在空间中的位置和方向。
#include "G4PVPlacement.hh"//定义一个物理体放置类，用于将逻辑体放置在空间中，G4PVPlacement 是 G4VPhysicalVolume 的一个具体子类，用来把一个 G4LogicalVolume 放进另一个体积中。
#include "G4Material.hh"//定义一个材料类，用于描述物质的属性。

#include "G4NistManager.hh"//从 Geant4 内置的 NIST 数据库中快速获取常用元素和材料，避免你手动定义每种元素、密度、原子量、组分比例等。
#include "G4SystemOfUnits.hh"//G4SystemOfUnits.hh 用来引入 Geant4 的 单位系统，有了它，你才能在代码中直接写：1.0*m，10.0*cm，5.0*mm，6.0*MeV，1.0*GeV，1.0*g/cm3，300.0*kelvin
#include "G4UnitsTable.hh"//G4UnitsTable.hh 和单位的 输出、显示、单位表管理、自定义单位 有关，比如直接写G4cout << stepLength << G4endl;看到的是 Geant4 内部单位下的数值，不一定直观，但如果写：G4cout << G4BestUnit(stepLength, "Length") << G4endl;Geant4 会自动选择合适单位。

#include "G4VisAttributes.hh"//定义一个可视化属性类，用于设置几何体的颜色、线条样式、填充样式等视觉属性，以便在 Geant4 的可视化系统中更好地展示几何结构。
#include "G4Color.hh"//定义一个颜色类，用于表示颜色的 RGBA 值，可以用来设置几何体的颜色属性。
#include "G4SDManager.hh"//定义一个敏感探测器管理类，用于管理和注册敏感探测器，敏感探测器是 Geant4 中用于记录粒子与几何体交互信息的组件。

#include "PMSensitiveDetector.hh"

class PMDetectorConstruction : public G4VUserDetectorConstruction
{
public:
    PMDetectorConstruction();//构造函数，通常在这里初始化一些成员变量或者设置默认值。
    virtual ~PMDetectorConstruction();//析构函数，负责清理资源，释放内存等。

    virtual G4VPhysicalVolume *Construct();//它负责建立几何结构，并返回世界物理体积，这是 Geant4 模拟的起点，所有其他几何体都必须放置在这个世界体积内。

private:
    G4LogicalVolume *logicDetector;//这个指针保存的是你的探测器逻辑体，它可能会在 Construct() 里面被赋值，然后在 ConstructSDandField() 里面使用，这个变量是类内部用来连接两个函数的，它不应该被外部随便访问或修改，不然可能会导致程序出错或者行为异常。

    virtual void ConstructSDandField();//这个函数负责构造敏感探测器和场，虽然这个函数会被 Geant4 调用，但你一般不希望用户代码直接调用它。
};

#endif