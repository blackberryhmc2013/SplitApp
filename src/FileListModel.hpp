/* Copyright (c) 2013-2014 BlackBerry Limited.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef FILELISTMODEL_HPP
#define FILELISTMODEL_HPP

#include <bb/cascades/QListDataModel>
#include <bb/system/InvokeManager>
#include <bb/system/InvokeRequest>
#include <bb/system/InvokeTargetReply>
#include <Quuid>

/*!
 * @brief A custom model that holds the file paths that shall be shared.
 */
class FileListModel : public bb::cascades::QStringListDataModel
{
    Q_OBJECT

public:
    /*!
     * Creates a new file list model.
     *
     * @param parent The parent object.
     */
    FileListModel(QObject *parent = 0);

    /*!
     * Returns the list of all files.
     */
    QList<QUrl> files() const;
private:
    bb::system::InvokeTargetReply* _invokeTargetReply;
public Q_SLOTS:
    /*!
     * Adds the given @p filePath to the model.
     */
    void addFile(const QString &filePath, QString uuid);

    /*!
     * Removes the given @p filePath from the model.
     */
    void removeFile(const QString &filePath);
    void onInvokeResult();

Q_SIGNALS:
    /*!
     * This signals is emitted whenever a file has been added or removed to/from the model.
     */
    void changed();
};

#endif
