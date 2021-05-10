import pygame
from tkinter.messagebox import *
from tkinter import Tk
from random import randrange as rnd

pygame.init()

#музыка в игре
pygame.mixer.music.load('zastavka.mp3')
pygame.mixer.music.play(-1, 0.0, 0)

shoot_sound = pygame.mixer.Sound('udar.mp3')
lose_sound = pygame.mixer.Sound('lose.mp3')
win_sound = pygame.mixer.Sound('win.mp3')

flPause = False

#параметры экрана
WIDTH, HEIGHT = 1200, 800
fps = 60

#параметры платформы
paddle_w = 330
paddle_h = 35
paddle_speed = 15
paddle = pygame.Rect(WIDTH // 2 - paddle_w // 2, HEIGHT - paddle_h - 10, paddle_w, paddle_h)

#параметры шарика
ball_radius = 20
ball_speed = 6
ball_rect = int(ball_radius * 2 ** 0.5)
ball = pygame.Rect(rnd(ball_rect, WIDTH - ball_rect), HEIGHT // 2, ball_rect, ball_rect)
dx, dy = 1, -1

#блоки
block_list = [pygame.Rect(10 + 120 * i, 40 + 70 * j, 100, 50) for i in range(10) for j in range(4)]
color_list = [(rnd(30,256), rnd(30,256), rnd(30, 256)) for i in range(10) for j in range(4)]

#создание экрана + заставка
sc = pygame.display.set_mode((WIDTH, HEIGHT))
clock = pygame.time.Clock()
img = pygame.image.load('oboi.jpg').convert()

#функция соударений мячика и блоков
def detect_collision(dx, dy, ball, rect):
    if dx > 0:
        delta_x = ball.right - rect.left
    else:
        delta_x = rect.right - ball.left
    if dy > 0:
        delta_y = ball.bottom - rect.top
    else:
        delta_y = rect.bottom - ball.top

    if abs(delta_x - delta_y) < 10:
        dx, dy = -dx, -dy
    elif delta_x > delta_y:
        dy = -dy
    elif delta_y > delta_x:
        dx = -dx

    return dx, dy

#очки
number_of_points = 0

#удаление окна Tk
root = Tk()
root.withdraw()

#цикл игры
while True:

    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            exit()
    sc.blit(img,(0,0))

    #вывод количества очков
    font = pygame.font.Font(None, 30)
    points = "Number of points: " + str(number_of_points)
    text = font.render(points, True, (255, 255, 255))
    place = text.get_rect(center=(100, 20))
    sc.blit(text, place)

    #создание блоков, платформы и шарика
    [pygame.draw.rect(sc, color_list[color], block) for color, block in enumerate(block_list)]
    pygame.draw.rect(sc, pygame.Color('red'), paddle)
    pygame.draw.circle(sc, pygame.Color('yellow'), ball.center, ball_radius)

    #движение шарика
    ball.x += ball_speed * dx
    ball.y += ball_speed * dy

    #изменение направления шарика при ударении со стенками
    if ball.centerx < ball_radius or ball.centerx > WIDTH - ball_radius:
        dx = -dx
    if ball.centery < ball_radius:
        dy = -dy

    #изменение направления шарика при ударении с платформой
    if ball.colliderect(paddle) and dy > 0:
        dx, dy = detect_collision(dx, dy, ball, paddle)

    # изменение направления шарика при ударении с блоками
    hit_index = ball.collidelist(block_list)
    if hit_index != -1:
        hit_rect = block_list.pop(hit_index)
        hit_color = color_list.pop(hit_index)
        dx, dy = detect_collision(dx, dy, ball, hit_rect)
        shoot_sound.play()

        hit_rect.inflate_ip(ball.width * 3, ball.height * 3)
        pygame.draw.rect(sc, hit_color, hit_rect)
        fps += 2
        number_of_points += 1

    #проигрыш или победа
    if ball.bottom > HEIGHT:
        pygame.mixer.music.stop()
        lose_sound.play()
        showinfo("RESULT", "GAME OVER!")
        print('GAME OVER!')
        exit()
    elif not len(block_list):
        pygame.mixer.music.stop()
        win_sound.play()
        showinfo("RESULT", "YOU WIN!")
        print('YOU WIN!!!')
        exit()

    #нажатие клавиш
    key = pygame.key.get_pressed()
    if key[pygame.K_LEFT] and paddle.left > 0:
        paddle.left -= paddle_speed
    if key[pygame.K_RIGHT] and paddle.right < WIDTH:
        paddle.right += paddle_speed
    if key[pygame.K_SPACE]:
        flPause = not flPause
        if flPause:
            pygame.mixer.music.pause()
        else:
            pygame.mixer.music.unpause()

    pygame.display.flip()
    clock.tick(fps)
