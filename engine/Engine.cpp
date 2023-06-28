#include "Engine.h"

Engine::Engine()
{
    window.create(sf::VideoMode::getDesktopMode(), "untitled", sf::Style::Fullscreen);
    camera.reset(window.getSize());
    worldRenderTexture.create(window.getSize().x, window.getSize().y);
    menuRenderTexture.create(window.getSize().x, window.getSize().y);
    inventory_menu.create(&player.inventory);
}

int Engine::run_engine()
{
    map.load();

    while(window.isOpen())
    {
        dt = clock.restart().asSeconds();

        worldTimeHolder.update(dt);
        player.update(dt, map.decor.wallBounds);
        entityHolder.update();
        camera.update(dt, player.get_bounds());
        map.clip(camera.getCameraRect());

        check_events();

        inventory_menu.update(camera.getCameraPosition(), camera.getMousePosition());

        draw();
    }

    return 0;
}

void Engine::check_events()
{
    sf::Event event{};

    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            window.close();

        if (event.type == sf::Event::MouseWheelScrolled)
            inventory_menu.transfuseItems(event.mouseWheelScroll.delta, camera.getMousePosition());
    }


    EventWatcher& eventWatcher = EventWatcher::getInstance();

    if (eventWatcher.isKeyBoardKeyTriggered(sf::Keyboard::I))
        inventory_menu.execute();
    if (eventWatcher.isKeyBoardKeyPressed(sf::Keyboard::E))
        entityHolder.entity_collect_check(&player);
}

void Engine::draw()
{
    /* Отрисовка World */
    worldRenderTexture.setView(camera.getView());
    worldRenderTexture.clear();
    worldRenderTexture.draw(map.terrain);
    worldRenderTexture.draw(map.decor);
    worldRenderTexture.draw(entityHolder);
    worldRenderTexture.draw(player);
    worldRenderTexture.draw(map.trees);
    worldRenderTexture.display();

    menuRenderTexture.setView(camera.getView());
    menuRenderTexture.clear(sf::Color::Transparent);
    menuRenderTexture.draw(inventory_menu);
    menuRenderTexture.display();

    sf::Sprite worldRenderSprite;
    worldRenderSprite.setColor(worldTimeHolder.get_world_color());
    worldRenderSprite.setTexture(worldRenderTexture.getTexture());

    sf::Sprite menuRenderSprite;
    menuRenderSprite.setTexture(menuRenderTexture.getTexture());

    window.clear();
    window.draw(worldRenderSprite);
    window.draw(menuRenderSprite);
    window.display();
}