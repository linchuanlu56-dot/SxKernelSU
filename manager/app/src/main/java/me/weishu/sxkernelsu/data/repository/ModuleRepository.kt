package com.sxkernelsu.data.repository

import com.sxkernelsu.data.model.Module
import com.sxkernelsu.data.model.ModuleUpdateInfo

interface ModuleRepository {
    suspend fun getModules(): Result<List<Module>>
    suspend fun checkUpdate(module: Module): Result<ModuleUpdateInfo>
}
