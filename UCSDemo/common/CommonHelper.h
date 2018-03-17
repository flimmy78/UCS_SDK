#ifndef COMMONHELPER_H
#define COMMONHELPER_H

#include <QImage>
#include <QObject>
#include <qglobal.h>

class CommonHelper
{
public:
    CommonHelper();

    /*!
     * \brief loadAppStyle 加载app样式表文件
     * \param style
     */
    static void loadAppStyle(const QString &style);

    /*!
     * \brief toGray 将彩色图片转换为灰度图片
     * \param image
     * \return
     */
    static QImage toGray(QImage image);

    /*!
     * \brief timeToString 将时间戳转换为指定格式时间字符串
     * \param time
     * \param format
     * \return
     */
    static QString timeToString(qint64 time, QString format);

    /*!
     * \brief tempDir 返回应用程序临时文件夹，若不存在则创建
     * \return 应用程序的临时文件夹
     */
    static QString tempDir();

    /*!
     * \brief userTempPath 返回每个用户的临时文件夹路径，若不存在则创建
     * \return
     */
    static QString userTempPath();

    /*!
     * \brief dataDir 返回应用程序数据文件夹，若不存在则创建
     * \return 应用程序数据存放文件夹
     */
    static QString dataDir();

    /*!
     * \brief pathForUserid 返回指定用户的本地目录路径
     * \return 返回每个userId专用目录路径
     */
    static QString userDataPath();

    /*!
     * \brief saveSetting 保存配置项
     * \param group
     * \param key
     * \param value
     */
    static void saveSetting(const QString &group,
                               const QString &key,
                               const QVariant &value);

    /*!
     * \brief readSetting 读取指定配置项
     * \param group
     * \param key
     * \param defaultVal
     * \return
     */
    static QVariant readSetting(const QString &group,
                                const QString &key,
                                const QVariant &defaultVal);

    /*!
     * \brief saveUserPwd 用户密码需要加密，单独函数保存
     * \param key 密码对应的key
     * \param pwd
     */
    static void saveUserPwd(QString key, QString pwd);
    static QString readUserPwd(QString key);
};

#endif // COMMONHELPER_H
