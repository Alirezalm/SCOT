from asyncio import subprocess
from json import dump, load
from fastapi import BackgroundTasks, FastAPI
from pydantic import BaseModel

from benchmark_old import time_benchmark

import os


class BenchmarkSettings(BaseModel):
    max_num_var: int = 100
    min_num_var: int = 10
    max_num_row: int = 50000
    min_num_row: int = 1000
    n_nodes: int = 4
    n_problems: int = 20
    pname: str = "benchmark"
    level_of_sparsity: float = 10


app = FastAPI()


@app.get("/")
async def index():
    return {"status": "online"}


@app.get('/get-status/')
async def get_status():

    if os.path.exists("results.json"):
        with open("results.json", 'r') as json_reader:
            data = load(json_reader)

        return {"results": data}

    return {"status": "computing ... !"}


@app.post("/benchmark/")
async def perform_benchmark(settings: BenchmarkSettings, back_task: BackgroundTasks):
    back_task.add_task(time_benchmark,
                       settings.max_num_var,
                       settings.min_num_var,
                       settings.max_num_row, settings.min_num_row,
                       settings.n_nodes,
                       settings.n_problems,
                       settings.pname,
                       settings.level_of_sparsity)

    response = {"results": "benchmark is started..."}

    return response


@app.get('/clean')
async def clean():
    if os.path.exists("results.json"):
        os.remove("results.json")
        subprocess.run([
            "rm",
            "/home/alireza/scotpy/inputs/*.dist.json"
        ])
        return {"status": "deleted"}
    return {"status": "file not exists"}
