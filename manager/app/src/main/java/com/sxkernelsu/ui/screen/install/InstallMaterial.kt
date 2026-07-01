package com.sxkernelsu.ui.screen.install

import androidx.compose.animation.AnimatedVisibility
import androidx.compose.animation.core.animateFloatAsState
import androidx.compose.animation.expandVertically
import androidx.compose.animation.fadeIn
import androidx.compose.animation.fadeOut
import androidx.compose.animation.shrinkVertically
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.WindowInsets
import androidx.compose.foundation.layout.WindowInsetsSides
import androidx.compose.foundation.layout.fillMaxHeight
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.foundation.layout.only
import androidx.compose.foundation.layout.padding
import androidx.compose.foundation.layout.safeDrawing
import androidx.compose.foundation.layout.safeDrawingPadding
import androidx.compose.foundation.rememberScrollState
import androidx.compose.foundation.verticalScroll
import androidx.compose.material.icons.Icons
import androidx.compose.material.icons.filled.Edit
import androidx.compose.material3.Icon
import androidx.compose.material3.LargeFlexibleTopAppBar
import androidx.compose.material3.SnackbarHost
import androidx.compose.material3.SnackbarHostState
import androidx.compose.material3.Text
import androidx.compose.material3.TopAppBarDefaults
import androidx.compose.material3.TopAppBarScrollBehavior
import androidx.compose.material3.rememberTopAppBarState
import androidx.compose.runtime.Composable
import androidx.compose.ui.Modifier
import androidx.compose.ui.input.nestedscroll.nestedScroll
import androidx.compose.ui.res.stringResource
import androidx.compose.material3.IconButton
import com.sxkernelsu.R
import com.sxkernelsu.ui.component.material.ExpressiveScaffold
import com.sxkernelsu.ui.component.material.SegmentedColumn
import com.sxkernelsu.ui.component.material.SegmentedDropdownItem
import com.sxkernelsu.ui.component.material.SegmentedListItem
import com.sxkernelsu.ui.component.material.TopBarBackButton
import com.sxkernelsu.ui.component.material.expressiveTopAppBarColors
import com.sxkernelsu.ui.theme.isInDarkTheme

/**
 * @author weishu
 * @date 2024/3/12.
 */
@Composable
internal fun InstallScreenMaterial(
    uiState: InstallUiState,
    actions: InstallScreenActions,
    snackBarHost: SnackbarHostState,
) {
    val scrollBehavior = TopAppBarDefaults.exitUntilCollapsedScrollBehavior(rememberTopAppBarState())

    ExpressiveScaffold(
        topBar = {
            TopBar(
                onBack = actions.onBack,
                scrollBehavior = scrollBehavior,
            )
        },
        snackbarHost = { SnackBarHost(hostState = snackBarHost, modifier = Modifier.safeDrawingPadding()) },
        contentWindowInsets = WindowInsets.safeDrawing.only(WindowInsetsSides.Top + WindowInsetsSides.Horizontal)
    ) { innerPadding ->
        Column(
            modifier = Modifier
                .padding(innerPadding)
                .fillMaxHeight()
                .nestedScroll(scrollBehavior.nestedScrollConnection)
                .verticalScroll(rememberScrollState())
        ) {
            SelectInstallMethod(
                state = uiState,
                onSelected = actions.onSelectMethod,
                onSelectBootImage = actions.onSelectBootImage,
            )

            SegmentedColumn(
                modifier = Modifier.padding(start = 16.dp, end = 16.dp, bottom = 13.dp),
                content = buildList {
                    if (uiState.displayPartitions.isNotEmpty()) add {
                        SegmentedDropdownItem(
                            enabled = uiState.canSelectPartition,
                            items = uiState.displayPartitions,
                            selectedIndex = uiState.partitionSelectionIndex,
                            title = "${stringResource(R.string.install_select_partition)} (${uiState.slotSuffix})",
                            onItemSelected = actions.onSelectPartition,
                            icon = Icons.Filled.Edit
                        )
                    }
                    add {
                        SegmentedListItem(
                            leadingContent = {
                                Icon(
                                    imageVector = Icons.Filled.Edit,
                                    contentDescription = null,
                                )
                            },
                            headlineContent = {
                                Text(text = stringResource(R.string.install_choose_lkm))
                            },
                            supportingContent = {
                                Text(text = uiState.lkmSelection?.title ?: stringResource(R.string.install_choose_lkm_option))
                            },
                            onClick = actions.onSelectLKM,
                        )
                    }
                }
            )
        }
    }
}

@Composable
private fun TopBar(
    onBack: () -> Unit = {},
    scrollBehavior: TopAppBarScrollBehavior? = null
) {
    LargeFlexibleTopAppBar(
        title = { Text(stringResource(R.string.install)) },
        navigationIcon = {
            TopBarBackButton(onClick = onBack)
        },
        colors = expressiveTopAppBarColors(),
        windowInsets = WindowInsets.safeDrawing.only(WindowInsetsSides.Top + WindowInsetsSides.Horizontal),
        scrollBehavior = scrollBehavior
    )
}
