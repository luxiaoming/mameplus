#include "qmc2main.h"

Utils *utils;

Utils::Utils(QObject *parent)
: QObject(parent)
{
	QFile icoFile(":/res/win_roms.ico");
	icoFile.open(QIODevice::ReadOnly);
	
	QByteArray data = icoFile.readAll();
	QBuffer buf(&data);
	buf.open(QIODevice::ReadOnly);
	deficon = loadWinIco(&buf);
	
	icoFile.close();

	spaceRegex = QRegExp("\\s+");
}

QString Utils::capitalizeStr(const QString & str)
{
	QStringList strlist = str.split("_");
	// capitalize first char
	for (int i=0; i < strlist.size(); i++)
		strlist[i][0] = strlist[i][0].toUpper();

	return strlist.join(" ");
}

QIcon Utils::loadIcon(const QString & gameName)
{
	QIcon icon = QIcon();

	QString dirpath = icons_directory;
	QDir dir(dirpath);
	dirpath = dir.path() + "/";	//clean it up

	// try to load directly
	QFile icoFile(dirpath + gameName + ".ico");
	if (icoFile.open(QIODevice::ReadOnly))
	{
		QByteArray data = icoFile.readAll();
		QBuffer buf(&data);
		buf.open(QIODevice::ReadOnly);
		icon = loadWinIco(&buf);
	}
	icoFile.close();
	
	if (icon.isNull())
	{
		// try to add .zip to nearest folder name
		QuaZip zip(dirpath + dir.dirName() + ".zip");
		if (zip.open(QuaZip::mdUnzip))
		{
			QuaZipFile *pZipFile = new QuaZipFile(&zip);
			if (zip.setCurrentFile(gameName + ".ico"))
			{
				if(pZipFile->open(QIODevice::ReadOnly))
				{
					QByteArray data = pZipFile->readAll();
					QBuffer buf(&data);
					buf.open(QIODevice::ReadOnly);
					icon = loadWinIco(&buf);
				}
				pZipFile->close();
			}
			delete pZipFile;
		}
		zip.close();
	}


	// recursively load parent image
	if (icon.isNull())
	{
		GameInfo *gameinfo = mamegame->gamenameGameInfoMap[gameName];
		if (!gameinfo->cloneof.isEmpty())
			icon = loadIcon(gameinfo->cloneof);

		// fallback to default image, first call can't reach here
		if (icon.isNull())
			icon = deficon;
	}
	return icon;
}

QIcon Utils::loadWinIco(QIODevice *device)
{
	QList<QImage> imgList = ICOReader::read(device);
	if (!imgList.isEmpty())
		return QIcon((QPixmap::fromImage(imgList.first())));
	else
		return QIcon();
}

QString Utils::getViewString(const QModelIndex &index, int column) const
{
	QModelIndex j = index.sibling(index.row(), column);
	return j.model()->data(j, Qt::DisplayRole).toString();
}

QPixmap Utils::getScreenshot(const QString &dirpath0, const QString &gameName, const QSize &size)
{
	QPixmap pm = QPixmap();
	QDir dir(dirpath0);
	QString dirpath = dir.path() + "/";	//clean it up

	// try to load directly	
	if (dir.exists() && !pm.load(dirpath + gameName + ".png"))
	{
		// try to add .zip to nearest folder name
		QuaZip zip(dirpath + dir.dirName() + ".zip");
		if (zip.open(QuaZip::mdUnzip))
		{
			QuaZipFile file(&zip);
			if (zip.setCurrentFile(gameName + ".png"))
			{
				if(file.open(QIODevice::ReadOnly))
				{
					QByteArray data = file.readAll();
					pm.loadFromData(data);
				}
				file.close();
			}
		}
		zip.close();
	}

	// recursively load parent image
	if (pm.isNull())
	{
		GameInfo *gameinfo = mamegame->gamenameGameInfoMap[gameName];
		if (!gameinfo->cloneof.isEmpty())
			pm = getScreenshot(dirpath, gameinfo->cloneof, size);

		// fallback to default image, first getScreenshot() can't reach here
		if (pm.isNull())
			pm = QPixmap(":/res/mamegui/about.png");
	}
	
	return pm.scaled(size, Qt::KeepAspectRatio, Qt::SmoothTransformation);
}

QString Utils::getHistory(const QString &gameName, const QString &fileName)
{
	QFile datFile(fileName);
	QString buf = "";

	if (datFile.open(QFile::ReadOnly | QFile::Text))
	{
		QTextStream in(&datFile);
		in.setCodec("UTF-8");

		bool recData = false;
		QString line;

		do
		{
			line = in.readLine().trimmed();
			if (!line.startsWith("#"))
			{
				if (line.startsWith("$"))
				{
					if (line.startsWith("$info="))
					{
						if (line.startsWith("$info=" + gameName))
							recData = true;
						else
						{
							if (recData)
								break;
							
							recData = false;
						}
					}
				}
				else if (recData)
				{
					buf += line;
					buf += "\n";
				}
			}
		}
		while (!line.isNull());
	}

	if (buf.trimmed().isEmpty())
	{
		GameInfo *gameInfo = mamegame->gamenameGameInfoMap[gameName];
		if (!gameInfo->cloneof.isEmpty())
			buf = getHistory(gameInfo->cloneof, fileName);
	}
	
	return buf.trimmed();
}


MyQueue::MyQueue(QObject *parent)
: QObject(parent)
{
	capacity = 1;
}

void MyQueue::setSize(int c)
{
	QMutexLocker locker(&mutex);
	capacity = c;
}

QString MyQueue::dequeue()
{
	QMutexLocker locker(&mutex);
//	emit logStatusUpdated(QString("deQueue: %1 %2").arg(queue.count()).arg(capacity));

	return queue.dequeue();
}

void MyQueue::enqueue(const QString & str)
{
	QMutexLocker locker(&mutex); 
	queue.enqueue(str);
	if (queue.count() > capacity)
		queue.dequeue();
	
//	emit logStatusUpdated(QString("enQueue: %1 %2").arg(queue.count()).arg(capacity));
}

bool MyQueue::isEmpty() const
{
	QMutexLocker locker(&mutex); 
	return queue.isEmpty();
}

