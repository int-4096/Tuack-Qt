#include <QCoreApplication>
#include <QCommandLineParser>
#include <QDebug>
#include <QDir>
#include <QSaveFile>
#include <QTextStream>
#include <QProcess>


// 辅助函数：创建文件并写入内容
bool writeFile(const QString &path, const QString &content)
{
    QSaveFile file(path);
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning() << "Cannot open" << path;
        return false;
    }
    file.write(content.toUtf8());
    if (!file.commit()) {
        qWarning() << "Failed to write" << path;
        return false;
    }
    return true;
}

void doInit()
{
    const QString problemDir = "problem";
    QDir dir;
    if (dir.exists(problemDir)) {
        qDebug() << "Directory" << problemDir << "already exists. Abort.";
        return;
    }

    dir.mkdir(problemDir);
    dir.cd(problemDir);
    dir.mkdir("data");
    dir.mkdir("solution");
    dir.mkdir("tools");

    // 1. 创建 statement.md
    QString statement = "";
    writeFile(dir.filePath("statement.md"), statement);

    // 2. 创建 config.json (最小示例)
    QString configJson = "";
    writeFile(dir.filePath("config.json"), "");

    // 3. 创建数据生成器模板 (data/gen.cpp)
    QString genCpp = "";
    writeFile(dir.filePath("data/gen.cpp"), genCpp);

    // 4. 创建标程模板 (solution/std.cpp)
    QString stdCpp = "";
    writeFile(dir.filePath("solution/std.cpp"), stdCpp);

    qDebug() << "Initialized problem at:" << dir.absolutePath();
}


bool compileToPdf(const QString &inputPath,
                             const QString &outputPath,
                             const QString &fontPath = "./fonts")
{
    // 1. 检查输入文件是否存在
    if (!QFileInfo::exists(inputPath)) {
        qCritical() << "[Typst] Input file does not exist:" << inputPath;
        return false;
    }

    // 2. 构造命令行参数
    QStringList args;
    args << "compile";

    // 如果指定了字体目录，添加 --font-path 参数
    if (!fontPath.isEmpty() && QDir(fontPath).exists()) {
        args << "--font-path" << fontPath;
        qDebug() << "[Typst] Using extra font path:" << fontPath;
    }

    args << inputPath;
    args << outputPath;

    // 3. 准备进程
    QProcess process;
    process.setProgram("typst");
    process.setArguments(args);

    // 设置工作目录为输入文件所在目录，便于相对路径引用（如 include 模板）
    QFileInfo inputInfo(inputPath);
    process.setWorkingDirectory(inputInfo.absolutePath());

    qDebug() << "[Typst] Command:" << process.program() << process.arguments().join(' ');

    // 4. 启动并等待完成
    process.start();
    if (!process.waitForStarted()) {
        qCritical() << "[Typst] Failed to start typst process:" << process.errorString();
        return false;
    }

    if (!process.waitForFinished()) {
        qCritical() << "[Typst] Typst process crashed or timeout:" << process.errorString();
        return false;
    }

    // 5. 检查退出码
    int exitCode = process.exitCode();
    QString stdOut = process.readAllStandardOutput();
    QString stdErr = process.readAllStandardError();

    if (exitCode != 0) {
        qCritical() << "[Typst] Compilation failed with exit code" << exitCode;
        if (!stdErr.isEmpty())
            qCritical() << "[Typst] stderr:\n" << stdErr;
        if (!stdOut.isEmpty())
            qDebug() << "[Typst] stdout:\n" << stdOut;
        return false;
    }

    // 6. 成功，输出详细信息（可选）
    qDebug() << "[Typst] Successfully generated PDF:" << outputPath;
    if (!stdErr.isEmpty())
        qDebug() << "[Typst] warnings:\n" << stdErr;

    return true;
}

void doRender()
{
    compileToPdf("statement.typ", "statement.pdf");
}

void doDump()
{

    qDebug() << "Exporting... (not implemented yet)";
}

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName("tuack");
    QCoreApplication::setApplicationVersion("0.1.0");

    QCommandLineParser parser;
    parser.setApplicationDescription("OI Problem Toolkit");
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption initOpt("init", "Initialize a new problem directory");
    QCommandLineOption renderOpt("render", "Render the statement");
    QCommandLineOption dumpOpt("dump", "Export for judge system (lemon/arbiter)");

    parser.addOption(initOpt);
    parser.addOption(renderOpt);
    parser.addOption(dumpOpt);

    parser.process(app);

    if (parser.isSet(initOpt)) {
        doInit();
    } else if (parser.isSet(renderOpt)) {
        doRender();
    } else if (parser.isSet(dumpOpt)) {
        doDump();
    } else {
        parser.showHelp();
    }
    return 0;
}