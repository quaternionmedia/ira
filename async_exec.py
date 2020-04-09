import asyncio

async def run(cmd):
    proc = await asyncio.create_subprocess_exec(
        *cmd, stdout=asyncio.subprocess.PIPE)

    data = await proc.stdout.read()
    await proc.wait()
    return data
