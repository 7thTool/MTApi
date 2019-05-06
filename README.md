# MTApi
支持Python/C/C++/http/websocket的行情、交易、指标、策略框架

MTApi是一套支持Python/C/C++/http/websocket的行情、交易、指标、策略系统开发框架，目标是服务国内外金融机构和专业量化投资用户，包括：公墓基金、私募基金、证券自营和资管、期货资管和子公司、高校研究机构、自营交易公司、交易所、专业投资者、金融创新企业等。

## 目标和特性

1. 全功能、跨平台行情交易开发平台，整合多种行情、交易接口，提供极低延迟实时行情，历史行情、指标数据，并提供简洁易用的API，用于快速构建行情交易应用，比如：股票/期货行情分析和交易应用、量化交易系统等

2. 覆盖国内外所有行情数据，行情数据使用C/C++基于共享内存，实现极地延迟、零拷贝，即使程序崩溃或者掉电不丢失数据，历史行情支持秒，1分、5分、15分、30分、时、天、季、年，以及自定义秒、分、时、天周期数据，提供统一易用的API交易接口：

    * CTP(ctp)：国内期货、期权

    * A股

    * 港股、美股

    * 全球证券、期货、期权、外汇等

    * 数字货币、现货、期货、期权、永续合约

3. 覆盖国内外所有交易品种的交易接口，交易使用C/C++基于共享内存，管里客户多市场多账户，提供统一易用的API交易接口：

    * CTP(ctp)：国内期货、期权

    * A股

    * 港股、美股

    * 全球证券、期货、期权、外汇等

    * 数字货币、现货、期货、期权、永续合约
    
4. 原生实现策略指标，使用C/C++在收到行情数据时，通过使用增量计算技术，基本上实现零消耗的情况下，讲行情和指标数据传给策略应用，从而实现极低延迟和极高效率的量化策略交易，完虐市场上所有量化策略平台

5. 自动开盘、运行策略、收盘，并自动落地历史数据

6. 开箱即用的各类指标和量化策略交易应用：
    
    * cta_strategy：CTA策略引擎模块，在保持易用性的同时，允许用户针对CTA类策略运行过程中委托的报撤行为进行细粒度控制（降低交易滑点、实现高频策略）

    * cta_backtester：CTA策略回测模块，无需使用Jupyter Notebook，直接使用图形界面直接进行策略回测分析、参数优化等相关工作

    * algo_trading：算法交易模块，提供多种常用的智能交易算法：TWAP、Sniper、Iceberg、BestLimit等等，支持常用算法配置保存

7. Python/C/C++行情、交易、指标API接口封装，以及支持http/websocket通过网络接入，方便各类应用接入与部署。

8. 简洁易用的事件驱动策略开发框架，方便策略开发。

9. [社区论坛](http://www.mtapi.com)和[知乎专栏](http://zhuanlan.zhihu.com/mtapi)，内容包括mtapi项目的开发教程和Python/C/C++在量化交易领域的应用研究等内容。

10. 官方交流群262656087（QQ），入群费将捐赠给mtapi社区基金。

## 环境准备

* 支持的系统版本：Windows 7以上/Windows Server 2008以上/Ubuntu 18.04 LTS
* 支持的Python版本：Python 3.7 64位

## 安装步骤

直接clone：git clone 

**Windows**

    install.bat

**Ubuntu**

    bash install.sh


## 使用指南

1. 在[SimNow](http://www.simnow.com.cn/)注册CTP仿真账号，并在[该页面](http://www.simnow.com.cn/product.action)获取经纪商代码以及交易行情服务器地址。

2. 在[MTApi社区论坛](https://www.mtapi.com/forum/)注册获得MTStation账号密码（论坛账号密码即是）

3. 启动MTStation，输入上一步的账号密码登录

4. MTStation里，您可以浏览行情、手动交易、增加和编辑指标、增加和编辑策略

注意：
* MTStation会启动MTApp、MTSrc、TDSrv等程序，通过共享内存共享数据，从而实现程序崩溃和掉电都不会丢失行情和交易数据，MTStation会24小时运行，自动开盘、运行策略、收盘，并自动落地历史数据


## 脚本运行

除了基于MTStation的图形化启动方式外，也可以在任意目录下创建run.py，写入以下示例代码：

```Python
from vnpy.event import EventEngine
from vnpy.trader.engine import MainEngine
from vnpy.trader.ui import MainWindow, create_qapp
from vnpy.gateway.ctp import CtpGateway
from vnpy.app.cta_strategy import CtaStrategyApp

def main():
    """Start VN Trader"""
    qapp = create_qapp()

    event_engine = EventEngine()
    main_engine = MainEngine(event_engine)
    
    main_engine.add_gateway(CtpGateway)
    main_engine.add_app(CtaStrategyApp)

    main_window = MainWindow(main_engine, event_engine)
    main_window.showMaximized()

    qapp.exec()

if __name__ == "__main__":
    main()
```

python执行：

    python run.py

## 贡献代码

mtapi使用Github托管其源代码，如果希望贡献代码请使用github的PR(Pull Request)的流程:

1. [创建 Issue](https://github.com/mtapi/mtapi/issues/new) - 对于较大的改动(如新功能，大型重构等)最好先开issue讨论一下，较小的improvement(如文档改进，bugfix等)直接发PR即可

2. Fork [mtapi](https://github.com/mtapi/mtapi) - 点击右上角**Fork**按钮

3. Clone你自己的fork: ```git clone https://github.com/$userid/mtapi.git```
	* 如果你的fork已经过时，需要手动sync：[https://help.github.com/articles/syncing-a-fork/](https://help.github.com/articles/syncing-a-fork/)

4. 从**dev**创建你自己的feature branch: ```git checkout -b $my_feature_branch dev```

5. 在$my_feature_branch上修改并将修改push到你的fork上

6. 创建从你的fork的$my_feature_branch分支到主项目的**dev**分支的[Pull Request] -  [在此](https://github.com/mtapi/mtapi/compare?expand=1)点击**compare across forks**，选择需要的fork和branch创建PR

7. 等待review, 需要继续改进，或者被Merge!

在提交代码的时候，请遵守以下规则，以提高代码质量：

  * 使用[autopep8](https://github.com/hhatto/autopep8)格式化你的代码。运行```autopep8 --in-place --recursive . ```即可。
  * 使用[flake8](https://pypi.org/project/flake8/)检查你的代码，确保没有error和warning。在项目根目录下运行```flake8```即可。


## 其他内容

* [获取帮助](https://github.com/mtapi/mtapi/blob/dev/docs/SUPPORT.md)
* [社区行为准侧](https://github.com/mtapi/mtapi/blob/dev/docs/CODE_OF_CONDUCT.md)
* [Issue模板](https://github.com/mtapi/mtapi/blob/dev/docs/ISSUE_TEMPLATE.md)
* [PR模板](https://github.com/mtapi/mtapi/blob/dev/docs/PULL_REQUEST_TEMPLATE.md)



## 版权说明
MIT
  




