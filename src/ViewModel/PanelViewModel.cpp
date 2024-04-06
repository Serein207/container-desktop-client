#include "PanelViewModel.h"
#include "Infrastructure/Cache/CacheManager.h"
#include "Infrastructure/Network/NetworkClient.h"
#include "Infrastructure/Utility/Result.hpp"
#include <qlist.h>
#include <qtypes.h>

PanelViewModel::PanelViewModel(QObject* parent) : QObject(parent) {}

void PanelViewModel::load(const QString& vmId, const QString& timeFrame) {
    if (vmId.toInt() < 1000) {
        emit loadSuccess();
        return;
    }
    ContainerDesktop::NetworkClient::getInstance()->getRrdData(
        CacheManager::getInstance()->getNodeForCurrentUser().value(), vmId, timeFrame,
        [this](Result<QList<RrdData>> result) {
            if (result.isErr()) {
                emit loadFailed(result.unwrapErr().message);
                return;
            }
            auto array = result.unwrap();
            QList<QString> timeList, cpuList, memoryList, netInList, netOutList;
            for (const auto& e : array) {
                timeList.append('\"' + e.time.toString("MM-dd-hh:mm") + '\"');
                cpuList.append(QString::number(int(e.cpu * 100 * 100) / 100.0));
                memoryList.append(QString::number(int(e.memUsed / e.memMax * 100 * 100) / 100.0));
                netInList.append(QString::number(int(e.netIn / 1024 * 100 * 100) / 100.0));
                netOutList.append(QString::number(int(e.netOut / 1024 * 100 * 100) / 100.0));
            }
            setCurCpu(cpuList.back() + "%");
            setCurMemory(memoryList.back() + "%");
            setCurNetIn(netInList.back() + "kB/s");
            setCurNetOut(netOutList.back() + "kB/s");
            setArrayTime("[" + timeList.join(',') + "]");
            setArrayCpu('[' + cpuList.join(',') + ']');
            setArrayMemory('[' + memoryList.join(',') + ']');
            setArrayNetIn('[' + netInList.join(',') + ']');
            setArrayNetOut('[' + netOutList.join(',') + ']');
            emit loadSuccess();
        });
}

QString PanelViewModel::getCurCpu() const {
    return curCpu;
}

void PanelViewModel::setCurCpu(const QString& newCurCpu) {
    if (curCpu == newCurCpu)
        return;
    curCpu = newCurCpu;
    emit curCpuChanged();
}

QString PanelViewModel::getCurMemory() const {
    return curMemory;
}

void PanelViewModel::setCurMemory(const QString& newCurMemory) {
    if (curMemory == newCurMemory)
        return;
    curMemory = newCurMemory;
    emit curMemoryChanged();
}

QString PanelViewModel::getCurNetIn() const {
    return curNetIn;
}

void PanelViewModel::setCurNetIn(const QString& newCurNetIn) {
    if (curNetIn == newCurNetIn)
        return;
    curNetIn = newCurNetIn;
    emit curNetInChanged();
}

QString PanelViewModel::getCurNetOut() const {
    return curNetOut;
}

void PanelViewModel::setCurNetOut(const QString& newCurNetOut) {
    if (curNetOut == newCurNetOut)
        return;
    curNetOut = newCurNetOut;
    emit curNetOutChanged();
}

QString PanelViewModel::getArrayTime() const {
    return arrayTime;
}

void PanelViewModel::setArrayTime(const QString& newArrayTime) {
    if (arrayTime == newArrayTime)
        return;
    arrayTime = newArrayTime;
    emit arrayTimeChanged();
}

QString PanelViewModel::getArrayCpu() const {
    return arrayCpu;
}

void PanelViewModel::setArrayCpu(const QString& newArrayCpu) {
    if (arrayCpu == newArrayCpu)
        return;
    arrayCpu = newArrayCpu;
    emit arrayCpuChanged();
}

QString PanelViewModel::getArrayMemory() const {
    return arrayMemory;
}

void PanelViewModel::setArrayMemory(const QString& newArrayMemory) {
    if (arrayMemory == newArrayMemory)
        return;
    arrayMemory = newArrayMemory;
    emit arrayMemoryChanged();
}

QString PanelViewModel::getArrayNetIn() const {
    return arrayNetIn;
}

void PanelViewModel::setArrayNetIn(const QString& newArrayNetIn) {
    if (arrayNetIn == newArrayNetIn)
        return;
    arrayNetIn = newArrayNetIn;
    emit arrayNetInChanged();
}

QString PanelViewModel::getArrayNetOut() const {
    return arrayNetOut;
}

void PanelViewModel::setArrayNetOut(const QString& newArrayNetOut) {
    if (arrayNetOut == newArrayNetOut)
        return;
    arrayNetOut = newArrayNetOut;
    emit arrayNetOutChanged();
}
