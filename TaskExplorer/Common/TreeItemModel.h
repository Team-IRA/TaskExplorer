#pragma once
#include "TreeViewEx.h"

class CTreeItemModel : public QAbstractItemModelEx
{
    Q_OBJECT

public:
    CTreeItemModel(QObject *parent = 0);
	virtual ~CTreeItemModel();

	void			SetTree(bool bTree)				{ m_bTree = bTree; }
	bool			IsTree() const					{ return m_bTree; }
	void			SetUseIcons(bool bUseIcons)		{ m_bUseIcons = bUseIcons; }

	//void			CountItems();
	QModelIndex		FindIndex(const QVariant& ID);
	void			RemoveIndex(const QModelIndex &index);

	QVariant		Data(const QModelIndex &index, int role, int section) const;

	// derived functions
    virtual QVariant		data(const QModelIndex &index, int role) const;
	virtual bool			setData(const QModelIndex &index, const QVariant &value, int role);
    virtual Qt::ItemFlags	flags(const QModelIndex &index) const;
    virtual QModelIndex		index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    virtual QModelIndex		parent(const QModelIndex &index) const;
    virtual int				rowCount(const QModelIndex &parent = QModelIndex()) const;
	virtual int				columnCount(const QModelIndex &parent = QModelIndex()) const = 0;
	virtual QVariant		headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const = 0;

public slots:
	void			Clear();

signals:
	void			CheckChanged(const QVariant& ID, bool State);
	void			ToolTipCallback(const QVariant& ID, QString& ToolTip) const;
	void			Updated();

protected:
	struct STreeNode
	{
		STreeNode(const QVariant& Id){
			ID = Id;
			Parent = NULL;
			Row = 0;
			//AllChildren = 0;

			IsBold = false;
			IsGray = false;
		}
		virtual ~STreeNode(){
			foreach(STreeNode* pNode, Children)
				delete pNode;
		}

		QVariant			ID;

		STreeNode*			Parent;
		int					Row;
		QList<QVariant>		Path;
		QList<STreeNode*>	Children;
		//int				AllChildren;
		QMap<QVariant, int>	Aux;

		QVariant			Icon;
		bool				IsBold;
		bool				IsGray;
		QColor				Color;
		struct SValue
		{
			QVariant Raw;
			QVariant Formated;
		};
		QVector<SValue>		Values;
	};

	virtual QVariant	NodeData(STreeNode* pNode, int role, int section) const;

	virtual STreeNode*	MkNode(const QVariant& Id) { return new STreeNode(Id); }

	void			Sync(QMap<QList<QVariant>, QList<STreeNode*> >& New, QHash<QVariant, STreeNode*>& Old);
	void			Purge(STreeNode* pParent, const QModelIndex &parent, QHash<QVariant, STreeNode*> &Old);
	void			Fill(STreeNode* pParent, const QModelIndex &parent, const QList<QVariant>& Paths, int PathsIndex, const QList<STreeNode*>& New, const QList<QVariant>& Path);
	QModelIndex		Find(STreeNode* pParent, STreeNode* pNode);
	//int				CountItems(STreeNode* pRoot);

	virtual QVariant GetDefaultIcon() const { return QVariant(); }

	STreeNode*							m_Root;
	QHash<QVariant, STreeNode*>			m_Map;
	bool								m_bTree;
	bool								m_bUseIcons;
};

class CSimpleTreeModel : public CTreeItemModel
{
	Q_OBJECT

public:
	CSimpleTreeModel(QObject *parent = 0) : CTreeItemModel(parent) {}
	
	void					Sync(const QMap<QVariant, QVariantMap>& List);

	void					setHeaderLabels(const QStringList& Columns) { m_Headers = Columns; }

	virtual int				columnCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant		headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

protected:
	QList<QVariant>			MakePath(const QVariantMap& Cur, const QMap<QVariant, QVariantMap>& List);
	bool					TestPath(const QList<QVariant>& Path, const QVariantMap& Cur, const QMap<QVariant, QVariantMap>& List, int Index = 0);

	QStringList				m_Headers;
};