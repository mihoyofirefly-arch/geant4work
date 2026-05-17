#include "PMDetectorConstruction.hh"

PMDetectorConstruction::PMDetectorConstruction()//构造函数，通常在这里初始化一些成员变量或者设置默认值。
{
}

PMDetectorConstruction::~PMDetectorConstruction()//析构函数，负责清理资源，释放内存等。
{
}

G4VPhysicalVolume *PMDetectorConstruction::Construct()//它负责建立几何结构，并返回世界物理体积，这是 Geant4 模拟的起点，所有其他几何体都必须放置在这个世界体积内，对应后面有写return physWorld;
{
    G4bool checkOverlaps = true;//这个变量用来控制是否检查几何体之间的重叠，设置为true会让Geant4在构建几何体时检查它们是否重叠，如果有重叠会输出警告信息，这对于调试几何结构非常有帮助，但在正式运行时可以设置为false以提高性能。

    G4NistManager *nist  = G4NistManager::Instance();//得到NIST材料管理器
    G4Material *worldMat = nist->FindOrBuildMaterial("G4_AIR");//定义世界体积材料为空气
    G4Material *leadMat = nist->FindOrBuildMaterial("G4_Pb");//定义铅材料，用于屏蔽
    G4Material *detMat = nist->FindOrBuildMaterial("G4_SODIUM_IODIDE");//定义探测器材料为碘化钠（Sodium Iodide），这是一种常用的闪烁体材料，可以用来检测辐射。

    // Define Fluorine-18（氟-18） source material
    G4Isotope *F18 = new G4Isotope("F18", 9, 18, 18.000938 * g / mole);//定义一个氟-18同位素，原子序数为9，核子数为18，原子量为18.000938克/摩尔
    G4Element *elF18 = new G4Element("Fluorine-18", "F18", 1);//定义一个元素，名字为Fluorine-18，符号为F18，包含1个同位素
    elF18->AddIsotope(F18, 100.0 * perCent);//将氟-18同位素添加到元素中，设置其相对丰度为100%，表示这个元素完全由氟-18同位素组成
    G4Material *matF18 = new G4Material("F18Source", 1.51 * g / cm3, 1);//定义一个材料，名字为F18Source，密度为1.51克/立方厘米，由1种元素组成
    matF18->AddElement(elF18, 100.0 * perCent);//将Fluorine-18元素添加到材料中，设置其质量分数为100%，表示这个材料完全由Fluorine-18元素组成

    G4double xWorld = 1. * m;
    G4double yWorld = 1. * m;
    G4double zWorld = 1. * m;//定义世界体积的尺寸，这里设置为1米立方，足够大以容纳所有其他几何体。

    G4Box *solidWorld = new G4Box("solidWorld", 0.5 * xWorld, 0.5 * yWorld, 0.5 * zWorld);//定义一个长方体几何体，名字为solidWorld，半长为0.5米，半宽为0.5米，半高为0.5米，这样整个世界体积就是1米立方。只表示形状
    G4LogicalVolume *logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicalWorld");//定义一个逻辑体，名字为logicalWorld，几何体为solidWorld，材料为worldMat（空气），这个逻辑体将被实例化为物理体。表示形状 + 材料
    G4VPhysicalVolume *physWorld = new G4PVPlacement(0, G4ThreeVector(0., 0., 0.), logicWorld, "physWorld", 0, false, 0, checkOverlaps);//第一个参数表示旋转矩阵，第二个参数表示表示放置位置，第三个参数是要被放置的逻辑体，第四个参数是物理体的名字，第五个参数表示母逻辑体（这里没有写0），第六个参数是pMany（它和早期 Geant4 中一些重复放置机制有关，现在固定写false），第七个参数是 copy number，如果你放置多个相同逻辑体，可以用 copy number 区分它们。表示这个体积真的放在空间里了

    // Fluorine source
    G4double sourceRadius = 1. * mm;//定义放射源的半径，这里设置为1毫米，表示这是一个小球形的放射源。

    G4Sphere *solidSource = new G4Sphere("solidSource", 0.0, sourceRadius, 0.0, 360. * deg, 0.0, 180. * deg);//定义一个球体几何体，名字为solidSource，内半径为0毫米，外半径为sourceRadius（1毫米），起始方位角为0度，覆盖360度，起始极角为0度，覆盖180度，这样就形成了一个完整的球体。只表示形状
    G4LogicalVolume *logicSource = new G4LogicalVolume(solidSource, matF18, "logicSource");//定义一个逻辑体，名字为logicSource，几何体为solidSource，材料为matF18（氟-18），这个逻辑体将被实例化为物理体。表示形状 + 材料
    G4VPhysicalVolume *physSource = new G4PVPlacement(0, G4ThreeVector(0., 0., -1. * cm), logicSource, "physSource", logicWorld, 0, checkOverlaps);//将放射源逻辑体放置在世界逻辑体中，位置为(0, 0, -1厘米)，也就是在世界中心的前面1厘米处。表示这个体积真的放在空间里了

    G4VisAttributes *sourceVisAtt = new G4VisAttributes(G4Color(1.0, 0.0, 1.0, 0.5));//定义一个可视化属性，颜色为紫色（红色1.0，绿色0.0，蓝色1.0），透明度为0.5，这样在可视化时这个放射源会以半透明的紫色显示。
    sourceVisAtt->SetForceSolid(true);//设置强制实心显示，这样即使在某些可视化模式下，放射源也会以实心的形式显示，而不是线框或点云。
    logicSource->SetVisAttributes(sourceVisAtt);//将这个可视化属性应用到放射源的逻辑体上，这样在可视化时这个逻辑体就会使用我们定义的颜色和显示样式。

    G4double leadThickness = 2. * mm;//定义铅屏蔽的厚度，这里设置为2毫米，表示这是一个薄的铅板，用来屏蔽放射源发出的辐射。
    G4double leadSize = 10. * cm;//定义铅屏蔽的尺寸，这里设置为10厘米，表示这个铅板是一个10厘米见方的正方形。
    G4Box *solidLead = new G4Box("solidLead", 0.5 * leadSize, 0.5 * leadSize, 0.5 * leadThickness);//定义一个长方体几何体，名字为solidLead，半长为0.5*leadSize（5厘米），半宽为0.5*leadSize（5厘米），半高为0.5*leadThickness（1毫米），这样就形成了一个10厘米见方、厚度为2毫米的铅板。只表示形状
    G4LogicalVolume *logicLead = new G4LogicalVolume(solidLead, leadMat, "logicLead");//定义一个逻辑体，名字为logicLead，几何体为solidLead，材料为leadMat（铅），这个逻辑体将被实例化为物理体。表示形状 + 材料
    G4VPhysicalVolume *physLead = new G4PVPlacement(0, G4ThreeVector(0., 0., 5. * cm), logicLead, "physLead", logicWorld, false, checkOverlaps);//将铅屏蔽逻辑体放置在世界逻辑体中，位置为(0, 0, 5厘米)，也就是在世界中心的后面5厘米处。表示这个体积真的放在空间里了

    G4VisAttributes *leadVisAtt = new G4VisAttributes(G4Color(1.0, 0.0, 0.0, 0.5));//定义一个可视化属性，颜色为红色（红色1.0，绿色0.0，蓝色0.0），透明度为0.5，这样在可视化时这个铅屏蔽会以半透明的红色显示。
    leadVisAtt->SetForceSolid(true);//设置强制实心显示，这样即使在某些可视化模式下，铅屏蔽也会以实心的形式显示，而不是线框或点云。
    logicLead->SetVisAttributes(leadVisAtt);//将这个可视化属性应用到铅屏蔽的逻辑体上，这样在可视化时这个逻辑体就会使用我们定义的颜色和显示样式。

    G4double detectorSize = 10.0 * cm;//定义探测器的尺寸，这里设置为10厘米，表示这是一个10厘米见方的正方形探测器。

    G4Box *solidDetector = new G4Box("solidDetector", 0.5 * detectorSize, 0.5 * detectorSize, 0.5 * detectorSize);//定义一个长方体几何体，名字为solidDetector，半长为0.5*detectorSize（5厘米），半宽为0.5*detectorSize（5厘米），半高为0.5*detectorSize（5厘米），这样就形成了一个10厘米见方的立方体探测器。只表示形状
    logicDetector = new G4LogicalVolume(solidDetector, detMat, "logicDetector");//定义一个逻辑体，名字为logicDetector，几何体为solidDetector，材料为detMat（碘化钠），这个逻辑体将被实例化为物理体。表示形状 + 材料
    G4VPhysicalVolume *physDetector = new G4PVPlacement(0, G4ThreeVector(0., 0. , 10.5 * cm), logicDetector, "physDetector", logicWorld, false, checkOverlaps);//将探测器逻辑体放置在世界逻辑体中，位置为(0, 0, 10.5厘米)，也就是在世界中心的后面10.5厘米处。表示这个体积真的放在空间里了

    G4VisAttributes *detVisAtt = new G4VisAttributes(G4Color(1.0, 1.0, 0.0, 0.5));//定义一个可视化属性，颜色为黄色（红色1.0，绿色1.0，蓝色0.0），透明度为0.5，这样在可视化时这个探测器会以半透明的黄色显示。
    detVisAtt->SetForceSolid(true);//设置强制实心显示，这样即使在某些可视化模式下，探测器也会以实心的形式显示，而不是线框或点云。
    logicDetector->SetVisAttributes(detVisAtt);//将这个可视化属性应用到探测器的逻辑体上，这样在可视化时这个逻辑体就会使用我们定义的颜色和显示样式。

    return physWorld;
}

void PMDetectorConstruction::ConstructSDandField()//这个函数负责构造敏感探测器和场，虽然这个函数会被 Geant4 调用，但你一般不希望用户代码直接调用它。
{
    PMSensitiveDetector *sensDet = new PMSensitiveDetector("SensitveDetector");//创建一个敏感探测器实例，名字为"SensitveDetector"，这个名字需要在后续的代码中用来注册和识别这个敏感探测器。
    logicDetector->SetSensitiveDetector(sensDet);//将这个敏感探测器关联到探测器的逻辑体上，这样当粒子经过这个逻辑体时，敏感探测器就会被触发，记录相关的物理信息。
    G4SDManager::GetSDMpointer()->AddNewDetector(sensDet);//将这个敏感探测器注册到全局的敏感探测器管理器中，这样 Geant4 就知道这个敏感探测器的存在，并且在模拟过程中会调用它来处理相关事件。
}//这段代码是在 给某个逻辑体 logicDetector 挂上敏感探测器 Sensitive Detector，当粒子进入 logicDetector 对应的探测器体积，并在里面发生 step 时，要调用 PMSensitiveDetector 来记录信息。