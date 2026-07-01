package com.sxkernelsu.data.repository

import com.sxkernelsu.data.model.RepoModule

interface ModuleRepoRepository {
    suspend fun fetchModules(): Result<List<RepoModule>>
}
