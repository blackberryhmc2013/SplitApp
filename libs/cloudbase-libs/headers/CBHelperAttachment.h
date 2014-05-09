/* Copyright (c) 2013 Cloudbase.io Limited
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
#ifndef CBHELPERATTACHMENT_H_
#define CBHELPERATTACHMENT_H_

namespace Cloudbase {

/**
 * This defines a file attachment for an insert/update request in the helper class.
 */
class CBHelperAttachment {
public:
	std::string fileName; /// The name of the file attached "example.jpg"
	std::string filePath; /// The full path of the file attached
	int fileSize; /// The size of the file attached
};

}


#endif /* CBHELPERATTACHMENT_H_ */
