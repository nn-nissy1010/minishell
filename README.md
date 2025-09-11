# minishell

## dockerコマンド

imageのビルド
docker build -t my-42-dev -f docker/Dockerfile .

コンテナのrun
docker run -it --name my-42-container -v $(pwd):/workspace my-42-dev

コンテナの削除
docker rm my-42-container          
