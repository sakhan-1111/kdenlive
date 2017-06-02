/***************************************************************************
 *   Copyright (C) 2017 by Nicolas Carion                                  *
 *   This file is part of Kdenlive. See www.kdenlive.org.                  *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) version 3 or any later version accepted by the       *
 *   membership of KDE e.V. (or its successor approved  by the membership  *
 *   of KDE e.V.), which shall act as a proxy defined in Section 14 of     *
 *   version 3 of the license.                                             *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 ***************************************************************************/

#include "effectitemmodel.hpp"

#include "effects/effectsrepository.hpp"
#include "effectstackmodel.hpp"
#include <utility>

EffectItemModel::EffectItemModel(const QList<QVariant> &data, Mlt::Properties *effect, const QDomElement &xml, const QString &effectId,
                                 const std::shared_ptr<AbstractTreeModel> &stack, const std::shared_ptr<TreeItem> &parent)
    : AbstractEffectItem(data, stack, parent)
    , AssetParameterModel(effect, xml, effectId)
{
}

// static
std::shared_ptr<EffectItemModel> EffectItemModel::construct(const QString &effectId, std::shared_ptr<AbstractTreeModel> stack, std::shared_ptr<TreeItem> parent)
{
    Q_ASSERT(EffectsRepository::get()->exists(effectId));
    QDomElement xml = EffectsRepository::get()->getXml(effectId);

    Mlt::Properties *effect = EffectsRepository::get()->getEffect(effectId);

    QList<QVariant> data;
    data << EffectsRepository::get()->getName(effectId) << effectId;

    std::shared_ptr<EffectItemModel> self(new EffectItemModel(data, effect, xml, effectId, std::move(stack), std::move(parent)));

    baseFinishConstruct(self);

    return self;
}

void EffectItemModel::plant(const std::weak_ptr<Mlt::Service> &service)
{
    if (auto ptr = service.lock()) {
        int ret = ptr->attach(filter());
        Q_ASSERT(ret == 0);
    } else {
        qDebug() << "Error : Cannot plant effect because parent service is not available anymore";
        Q_ASSERT(false);
    }
}

Mlt::Filter &EffectItemModel::filter() const
{
    return *static_cast<Mlt::Filter *>(m_asset.get());
}


void EffectItemModel::updateEnable()
{
    filter().set("disable", isEnabled() ? 0 : 1);
}
