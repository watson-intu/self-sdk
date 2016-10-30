/**
* Copyright 2016 IBM Corp. All Rights Reserved.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*      http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*
*/

#ifndef WDC_SHA1_H
#define WDC_SHA1_H

#include <openssl/sha.h>

template<class type>
void SHA1(const type& input, type& hash) 
{
	SHA_CTX context;
	SHA1_Init(&context);
	SHA1_Update(&context, &input[0], input.size());

	hash.resize(160 / 8);
	SHA1_Final((unsigned char*)&hash[0], &context);
}

template<class type>
type SHA1(const type& input) 
{
	type hash;
	SHA1(input, hash);
	return hash;
}

#endif
